#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
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

struct {
	schedule_entry * entries;
	int entry_count;

	unsigned allocated_memory : 1;
} _module;

ERR_CODE start_schedule(process_table * table, int entry_count) {
	/* Allocate space for entries */
	_module.entries = malloc(entry_count * sizeof(schedule_entry));
	_module.allocated_memory = 1;
	_module.entry_count = entry_count;

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

		/* Configure pipes */
		close(_module.entries[i].pipe[1]);
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
