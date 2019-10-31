#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include "../../include/processor.h"

typedef struct _schedule_entry {
	/* fork() and pipe() variables */
	int pid;
	int pipe[2];

	/* flag to determine if the process started */
	unsigned started : 1;
	unsigned ended : 1;

	unsigned int time_used;

	/* process_table reference */
	process_table * table;
} schedule_entry;

static struct {
	schedule_entry * entries;
	int entry_count;

	int timer_count;

	schedule_entry * current_entry;
	schedule_entry * last_entry;

	unsigned allocated_memory : 1;
	unsigned timer_dirty : 1;
} _module;

void handle_timer(int signal) {
	++_module.timer_count;
	_module.timer_dirty = 1;
}

ERR_CODE sort_entries() {
	/* Bubble sort */
	for(int i = 0; i < _module.entry_count - 1; ++i) {
		for(int j = i + 1; j < _module.entry_count; ++j) {
			if(_module.entries[j].table->arrival_time < _module.entries[i].table->arrival_time) {
				schedule_entry tmp = _module.entries[i];
				_module.entries[i] = _module.entries[j];
				_module.entries[j] = tmp;
			}
		}
	}
	return OK;
}

ERR_CODE start_schedule(process_table * table, int entry_count) {
	/* Allocate space for entries */
	_module.entries = malloc(entry_count * sizeof(schedule_entry));
	_module.allocated_memory = 1;
	_module.entry_count = entry_count;

	/* Set up timer to trigger every second */
	signal(SIGALRM, handle_timer);	
	struct itimerval timer;
	timer.it_value.tv_sec = 1;
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = 1;
	timer.it_interval.tv_usec = 0;
	setitimer ( ITIMER_REAL, &timer, NULL);

	/* start all the child processes */
	for(int i = 0; i < entry_count; ++i) {
		_module.entries[i].table = &(table[i]);
	}
	/* Sort the entries now to ensure highest priority is highest */
	sort_entries();
	/* print to ensure we're sorted */
	for(int i = 0; i < _module.entry_count; ++i) {
		printf("ID: %d\tPriority: %d\tArr_time:%d\n", _module.entries[i].table->process_number, _module.entries[i].table->priority, _module.entries[i].table->arrival_time);
	}
	return OK; // We're good
}

ERR_CODE start_entry(schedule_entry * entry) {
	/* Make sure pipe doesn't fail */
	if(pipe(entry->pipe) < 0) {
		return PIPE_FAILURE;
	}
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
	
	return OK;
}

ERR_CODE run_schedule() {
	/* If the dirty flag is set, we need to process this time */
	if(_module.timer_dirty) {
		/* Look at current entry and stop if burst is up */
		if(_module.current_entry != NULL &&
		 _module.current_entry->time_used >= _module.current_entry->table->burst) {
			kill(_module.current_entry->pid, SIGINT);
			waitpid(_module.current_entry->pid, NULL, 0);
			char primes[20];
			read(_module.current_entry->pipe[0], primes, 20);
			printf("Process %d closed: Prime found was %s\n", _module.current_entry->table->process_number, primes);
			_module.current_entry->ended = 1;
			_module.current_entry = NULL;
		}
		/* Now look for the highest priority entry that arrived soonest and execute it */
		_module.last_entry = _module.current_entry;
		for(int i = 0; i < _module.entry_count; ++i) {
			/* This checks if the entry hasn't ended yet, and if it's "arrived" yet */
			if(!_module.entries[i].ended && _module.entries[i].table->arrival_time <= _module.timer_count) {
				if(_module.current_entry == NULL) _module.current_entry = &(_module.entries[i]);
				if(_module.current_entry->table->priority > _module.entries[i].table->priority) 
					_module.current_entry = &(_module.entries[i]);
			}
		}
		/* If we updated the current entry, we have to stop the last one and start the new one */
		if(_module.last_entry != _module.current_entry) {
			if(_module.last_entry != NULL) {
				kill(_module.last_entry->pid, SIGTSTP);
				char primes[20];
				read(_module.last_entry->pipe[0], primes, 20);
				printf("Process %2d stopped:   PID %6d, time used %3d, current prime is %5s\n", 
						_module.last_entry->table->process_number, 
						_module.last_entry->pid,
						_module.last_entry->time_used,
						primes);
			}
			if(_module.current_entry->started) {
				kill(_module.current_entry->pid, SIGCONT);
				printf("Process %2d continued: PID %6d, time used %3d, priority %d, burst %d\n", 
						_module.current_entry->table->process_number,
						_module.current_entry->pid,
						_module.current_entry->time_used,
						_module.current_entry->table->priority,
						_module.current_entry->table->burst);
			} else {
				start_entry(_module.current_entry);
				printf("Process %2d started:   PID %6d,                priority %d, burst %d\n", 
						_module.current_entry->table->process_number,
						_module.current_entry->pid,
						_module.current_entry->table->priority,
						_module.current_entry->table->burst);
			}
		}
		_module.timer_dirty = 0;
		if(_module.current_entry != NULL) ++_module.current_entry->time_used;
	}

	/* Check every entry to see if there's any that haven't ended yet */
	int all_ended = 1;
	for(int i = 0; i < _module.entry_count; ++i) {
		if(!_module.entries[i].ended) all_ended = 0;
	}
	if(all_ended) return ALL_ENDED;
	return OK;
}


ERR_CODE stop_processor() {
	/* If we've allocated memory, we have processes running, so let's stop them */
	if(_module.allocated_memory) {
		/* Stop all children */
		for(int i = 0; i < _module.entry_count; ++i) {
			/* If this process is running, stop it */
			if(_module.entries[i].started && !_module.entries[i].ended) {
				kill(_module.entries[i].pid, SIGINT);
				waitpid(_module.entries[i].pid, NULL, 0);
			}
			_module.entries[i].started = 0;
		}
	}
	return OK;
}

ERR_CODE free_processor() {
	/* If we've allocated memory, then kill everything */
	if(_module.allocated_memory) {	
		/* Free Malloc'd memory */
		free(_module.entries);
		/* Set flag for allocated memory to false */
		_module.allocated_memory = 0;
	}
	return OK;
}
