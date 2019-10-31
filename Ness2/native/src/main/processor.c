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

	/* process_table reference */
	process_table * table;
} schedule_entry;

static struct {
	schedule_entry * entries;
	int entry_count;

	int timer_count;

	unsigned allocated_memory : 1;
} _module;

void handle_timer(int signal) {
	if(_module.timer_count) {
		++_module.timer_count;
	}
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
		/* Make sure pipe doesn't fail */
		if(pipe(_module.entries[i].pipe) < 0) {
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
			close(_module.entries[i].pipe[0]);
			dup2(_module.entries[i].pipe[1], STDOUT_FILENO);
			close(_module.entries[i].pipe[1]);

			/* Start with execv */
			char *args[] = {"./prime", NULL};
			execv(args[0], args);
			exit(-1); // Exit with failure if we make it this far
		}
		/* This is parent process */
		_module.entries[i].pid = pid;
		_module.entries[i].started = 1;

		/* Configure pipes */
		close(_module.entries[i].pipe[1]);
	}
	/* Sort the entries now to ensure highest priority is highest */
	sort_entries();
	/* print to ensure we're sorted */
	for(int i = 0; i < _module.entry_count; ++i) {
		printf("ID: %d\tPriority: %d\tArr_time:%d\n", _module.entries[i].table->process_number, _module.entries[i].table->priority, _module.entries[i].table->arrival_time);
	}
	return OK; // We're good
}

ERR_CODE stop_processor() {
	/* If we've allocated memory, we have processes running, so let's stop them */
	if(_module.allocated_memory) {
		/* Stop all children */
		for(int i = 0; i < _module.entry_count; ++i) {
			/* If this process is running, stop it */
			if(_module.entries[i].started) {
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
