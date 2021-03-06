/**
 * Author: Cory
 * Assignment Number: 2
 * Date of Submission: 11/6/2019
 * Name of this file: scheduler.c
 * Description of the program:
 *  This program runs a scheduler on a set of prime-finders
 *  in order to show how a priority scheduler works
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "../../include/fileparser.h"
#include "../../include/processor.h"

#define USAGE "schedule <filename>"

/* Local struct that holds stateful data */
static struct {
	process_table * table;
	int table_entries;

	unsigned keep_alive : 1;
} _module;

/** 
 * handle_sigint
 * Purpose: Handle interrupt signal
 * Parameters:
 *  sig - interrupt used to call this
 */
void handle_sigint(int sig) {
	_module.keep_alive = 0;
	printf("Closing...\n");
}

/**
 * init_module
 * Purpose: Initialize the module
 */
void init_module() {
	signal(SIGINT, handle_sigint);
}

int main(int argc, char **argv) {
	/* Make continuous error return */
	ERR_CODE err = OK;
	/* Check correct number of parameters */
	if(argc != 2) {
		fprintf(stderr, 
			"Invalid parameter count! usage is: " USAGE "\n");
		exit(-1);
	}

	/* Initialize module */
	init_module();
	
	/* Parse file and fill table */
	err = parse_file(argv[1], &_module.table, &_module.table_entries);
	/* Start schedule */
	if(err == OK) {
		err = start_schedule(_module.table, _module.table_entries);
	}
	/* Wait until SIGINT is called */
	if(err == OK) {
		_module.keep_alive = 1;
		while(_module.keep_alive) {
			ERR_CODE ret = run_schedule();
			if(ret == ALL_ENDED) break;
			pause();
		}
		/* SIGINT is called, let's stop processes */
		stop_processor();
	}

	/* Print errors if there are errors */
	switch(err) {
		case OK: break; //!< Everything OK
		case INVALID_FILE:
			fprintf(stderr, "File not found\n");
			break;
		case INVALID_FILE_FORMAT:
			fprintf(stderr, "File format is incorrect\n");
			break;
		
		default:
			fprintf(stderr, "General error: %d\n", err);
			break;
	}

	/* We have to free memory that's dynamically allocated */
	free_processor();
	free(_module.table);
	/* If we had an error, return Failure */
	if(err) { exit(-1); }
	/* Otherwise we return Success */
	else { exit(0); }
}


