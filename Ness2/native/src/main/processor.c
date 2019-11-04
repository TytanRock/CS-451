#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include "../../include/processor.h"

/* String used for table header */
#define HEADER_STRING "Time    State     PID   ID  Priority  Number\n"
/* String used for content of entry */
#define CONTENT_STRING "%6d  %2d  %9d  %-s\n"
/* Length of string to read from child */
#define STR_LEN 20 


/* Structure to hold process details for this module specifically */
typedef struct _schedule_entry {
	/* fork() and pipe() variables */
	int pid; //!< PID Number of process forked
	int pipe[2]; //!< Pipe details for inter-process communication

	/* flag to determine if the process started */
	unsigned started : 1; //!< 1 if started, 0 otherwise
	unsigned ended : 1; //!< 1 if ended, 0 otherwise

	unsigned int time_used; //!< time in seconds this process has been running for

	/* process_table reference */
	process_table * table; //!< Holds process_table variables (process_number, priority, burst, arrival_time)
} schedule_entry;

/* Local structure to hold all our global variables */
static struct {
	schedule_entry * entries; //!< Array of entries used in module
	int entry_count; //!< Number of entries

	int timer_count; //!< Total time spent in process

	schedule_entry * current_entry; //!< Reference to currently used entry
	schedule_entry * last_entry; //!< Reference to last uesd entry

	unsigned allocated_memory : 1; //!< 1 if memory has been allocated for entries, 0 otherwise
	unsigned timer_dirty : 1; //!< 1 if timer interrupt has just fired, 0 otherwise
} _module;

/** 
 * handle_timer
 * Purpose: handle the timer interrupt by setting the timer_dirty flag and 
 * incrementing timer_count
 * Parameters:
 *  signal - type of signal that triggered interrupt
 */
void handle_timer(int signal) {
	++_module.timer_count;
	_module.timer_dirty = 1;
}

/**
 * sort_entries
 * Purpose: Sort entries by arrival time
 * Completely side-effectfull
 * Returns: Error code based on success of sort or not
 */
ERR_CODE sort_entries() {
	/* If module hasn't allocated memory, return an error */
	if(!_module.allocated_memory) {
		return MEMORY_NOT_ALLOCATED;
	}
	/* Bubble sort temporarily */
	for(int i = 0; i < _module.entry_count - 1; ++i) {
		for(int j = i + 1; j < _module.entry_count; ++j) {
			if(_module.entries[j].table->arrival_time < _module.entries[i].table->arrival_time) {
				schedule_entry tmp = _module.entries[i];
				_module.entries[i] = _module.entries[j];
				_module.entries[j] = tmp;
			}
		}
	}
	return OK; //!< Successful sort
}

/**
 * start_schedule
 * Purpose: initialize module for future
 * Parameters:
 *  table - Array of tables of processes to run
 *  entry_count - Number of tables in array
 * Returns: Error code based on startup state
 */
ERR_CODE start_schedule(process_table * table, int entry_count) {
	/* Allocate space for entries */
	_module.entries = malloc(entry_count * sizeof(schedule_entry));
	_module.allocated_memory = 1;
	_module.entry_count = entry_count;

	/* Set up timer to trigger every second */
	signal(SIGALRM, handle_timer); //!< Configure handle_timer as the timer handler
	struct itimerval timer;
	timer.it_value.tv_sec = 1; //!< 1 second
	timer.it_value.tv_usec = 0; //!< 0 useconds
	timer.it_interval.tv_sec = 1; //!< 1 second
	timer.it_interval.tv_usec = 0; //!< 0 useconds
	setitimer ( ITIMER_REAL, &timer, NULL); //!< Configure timer with struct

	/* Copy every table into every entry */
	for(int i = 0; i < entry_count; ++i) {
		_module.entries[i].table = &(table[i]);
	}
	/* Sort the entries now to ensure it's in arrival order */
	sort_entries();

	/* Print header */
	printf(HEADER_STRING);
	return OK; // We're good
}

/**
 * start_entry
 * Purpose: start process for specified entry
 * Parameters:
 *  entry - Entry to start process with
 * Returns: Error code based on success of starting entry
 */
ERR_CODE start_entry(schedule_entry * entry) {
	/* Make sure pipe doesn't fail */
	if(pipe(entry->pipe) < 0) {
		return PIPE_FAILURE;
	}
	/* fork process and save pid */
	int pid = fork();	
	/* Make sure fork doesn't fail */
	if(pid < 0) {
		return FORK_FAILURE;
	}
	/* Check to see if we're child */
	if(pid == 0) {
		/* Set up STDOUT */
		close(entry->pipe[0]);
		dup2(entry->pipe[1], STDOUT_FILENO);
		close(entry->pipe[1]);

		/* Start with execv */
		char *args[] = {"./bin/prime", NULL};
		execv(args[0], args);
		exit(-1); // Exit with failure if we make it this far
	}
	/* This is parent process */
	entry->pid = pid;
	entry->started = 1;

	/* Configure pipes */
	close(entry->pipe[1]);
	
	return OK; //!< We successfully started, return OK
}

/**
 * run_schedule
 * Purpose: check if we've triggered our 1 second timer,
 *  if we have, let's run our scheduling algorithm,
 *  otherwise, let's immediately return
 * Returns: Error code based on success of function
 */
ERR_CODE run_schedule() {
	/* If the dirty flag is set, we need to process this time */
	if(_module.timer_dirty) {
		/* Flags that determine if we need to print something */
		int process_stopped = 0;
		int process_started = 0;
		int process_halted = 0;
		int process_resumed = 0;
		schedule_entry * stopped_entry; //!< Reference to entry if we stop it

		/* Look at current entry and stop if burst is up */
		if(_module.current_entry != NULL &&
		 _module.current_entry->time_used >= _module.current_entry->table->burst) {
			kill(_module.current_entry->pid, SIGINT); //!< Send SIGINT signal
			waitpid(_module.current_entry->pid, NULL, 0); //!< Ensure process exits
			_module.current_entry->ended = 1; //!< Set flag so entry is ended
			stopped_entry = _module.current_entry; //!< Reference our local stopped entry to this
			process_stopped = 1; //!< Set flag so we can print stopped message
			_module.current_entry = NULL; //!< Set current entry to NULL to get next best entry
		}
		/* Now look for the highest priority entry that arrived soonest and execute it */
		_module.last_entry = _module.current_entry;
		/* Look at every entry to find highest priority that's available */
		for(int i = 0; i < _module.entry_count; ++i) {
			/* If entry hasn't ended, and it's available, we need to check it */
			if(!_module.entries[i].ended && _module.entries[i].table->arrival_time <= _module.timer_count) {
				/* If current entry is NULL, let's set current entry to this entry */
				if(_module.current_entry == NULL) 
					_module.current_entry = &(_module.entries[i]);
				/* If this entry is a higher priority than our current entry, switch to it */
				if(_module.current_entry->table->priority > _module.entries[i].table->priority) 
					_module.current_entry = &(_module.entries[i]);
			}
		}
		/* If we updated the current entry, we have to stop the last one and start the new one */
		if(_module.last_entry != _module.current_entry) {
			/* NULL check our last entry */
			if(_module.last_entry != NULL) {
				kill(_module.last_entry->pid, SIGTSTP); //!< Send a halting signal
				process_halted = 1; //!< Make sure flag is set
			}
			/* If current entry is started, than we continue */
			if(_module.current_entry->started) {
				kill(_module.current_entry->pid, SIGCONT); //!< Sends continue signal
				process_resumed = 1; //!< Make sure flag is set
			/* Otherwise we need to start it */
			} else {
				start_entry(_module.current_entry); //!< Start process with specified entry
				process_started = 1; //!< Make sure flag is set
			}
		}

		/* Print state information now */
		do {
			/* Start with time */
			printf("%-4d  ", _module.timer_count);
			
			/* If process is stopped, print stopped information */
			if(process_stopped) {
				char tmp[STR_LEN]; //!< Initialize temporary string
				memset(tmp, 0, STR_LEN); //!< Clear our temporary string
				read(stopped_entry->pipe[0], tmp, STR_LEN); //!< Read child process output
				printf("  Ended  " CONTENT_STRING, 
						stopped_entry->pid, 
						stopped_entry->table->process_number,
						stopped_entry->table->priority,
						tmp); //!< Print out our content
				process_stopped = 0; //!< Clear flag
				continue; //!< Important because we want a new entry for every flag
			}
			/* If a process halted, print halted information */
			if(process_halted) {
				char tmp[STR_LEN]; //!< Initialize temporary string
				memset(tmp, 0, STR_LEN); //!< Clear our temporary string
				read(_module.last_entry->pipe[0], tmp, STR_LEN); //!< Read child process output
				printf("Suspend  " CONTENT_STRING,
						_module.last_entry->pid,
						_module.last_entry->table->process_number,
						_module.last_entry->table->priority,
						tmp); //!< Print out our content
				process_halted = 0; //!< Clear flag
				continue; //!< Important because we want a new entry for every flag
			}
			/* If a process started, print started information */
			if(process_started) {
				printf("Started  " CONTENT_STRING,
						_module.current_entry->pid,
						_module.current_entry->table->process_number,
						_module.current_entry->table->priority,
						""); //!< Print out our content
				process_started = 0; //!< Clear flag
				continue; //!< Important because we want a new entry for every flag
			}
			/* If a process resumed, print resumed information */
			if(process_resumed) {
				printf("Resumed  " CONTENT_STRING,
						_module.current_entry->pid,
						_module.current_entry->table->process_number,
						_module.current_entry->table->priority,
						""); //!< Print out our content
				process_resumed = 0; //!< Clear flag
				continue; //!< Important because we want a new entry for every flag
			}

			printf("\n"); //!< We haven't printed out information, add a newline and move on

			/* We continue printing table entries until we've ran out of entries to print (*/
		} while(process_stopped || process_started || process_halted || process_resumed);

		/* Clear dirty flag */
		_module.timer_dirty = 0;
		/* If our current entry is available, increment the time it's been running for */
		if(_module.current_entry != NULL) ++_module.current_entry->time_used;
	}

	/* Check every entry to see if there's any that haven't ended yet */
	int all_ended = 1;
	for(int i = 0; i < _module.entry_count; ++i) {
		if(!_module.entries[i].ended) all_ended = 0;
	}
	if(all_ended) return ALL_ENDED; //!< If everything is ended, let caller know so we can exit
	return OK; //!< Otherwise return OK cause we made it
}

/**
 * stop_processor
 * Purpose: Stop all children processes
 * Completely side-effectful
 * Returns: Error code based on success of function
 */
ERR_CODE stop_processor() {
	/* If we've allocated memory, we have processes running, so let's stop them */
	if(_module.allocated_memory) {
		/* Stop all children */
		for(int i = 0; i < _module.entry_count; ++i) {
			/* If this process hasn't ended, stop it */
			if(_module.entries[i].started && !_module.entries[i].ended) {
				kill(_module.entries[i].pid, SIGINT); //!< Send SIGINT signal
				waitpid(_module.entries[i].pid, NULL, 0); //!< Wait for it to finish
			}
			_module.entries[i].ended = 1;
		}
	}
	return OK; //!< Always successfully stops processes
}

/**
 * free_processor
 * Purpose: Clean up allocated memory in module
 * Completely side-effectful
 * Returns: Erorr code based on success of function
 */
ERR_CODE free_processor() {
	/* If we've allocated memory, then kill everything */
	if(_module.allocated_memory) {	
		/* Free Malloc'd memory */
		free(_module.entries);
		/* Set flag for allocated memory to false */
		_module.allocated_memory = 0;
	}
	return OK; //!< Always successfully frees
}
