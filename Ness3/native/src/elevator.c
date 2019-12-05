/**
 * Author: Cory Ness
 * Assignment Number: 3
 * Date of Submission: 12/5/2019
 * Name of this file: elevator.c
 * Description of program:
 *  This program runs an elevator and n number of people
 *  The people get on and off the elevator and move to specified floors
 *  They wait for some number of seconds at each floor, and eventually leave the building
 * This project uses 1 coupon
 */

#include "../include/lift.h"
#include "../include/person.h"
#include "../include/global.h"
#include "../include/printing.h"

#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define VALID_OPTS "p:w:f:h"

#define USAGE "usage: elevator [-p <people>] [-w <wait time>] [-f <max floor>] [-h] ...\n \
	-p <people> - The number of people that will be using elevator\n \
	-w <wait time> - The maximum wait time of the people\n \
	-f <max floor> - The maximum floor the elevator goes to\n \
	-h - Display this help information\n \
	... - Each person's time-pairs, passed into stdin, rather than the command line\n"

/* Local struct holding module-variables */
static struct {
	pthread_t lift_thread; //!< Singular thread for the lift thread
	pthread_t *people_threads; //!< Array of threads for people

	person_time *people_time; //!< Array of time-floor pairs for each person
} _module;

/**
 * find_options
 * Purpose:
 *  Update module struct based on CLI parameters passed
 *
 * Parameters:
 *  argc - argument count as passed from main
 *  args - argument strings as passed from main
 */
void find_options(const int argc, char **args) {
	char opt; //!< Character of the option passed
	
	/* Get all arguments from the command line */
	while((opt = getopt(argc, args, VALID_OPTS)) != -1) {
		switch(opt) {
			case 'p': // People argument
				/* If the optarg is invalid, this returns 0 which is handled later on in the program */
				_global.total_people = atoi(optarg);
				break;	
			case 'w': // Wait argument
				_global.max_wander_time = atoi(optarg);
				break;
			case 'f': // Floor argument
				_global.max_floor = atoi(optarg);
				break;
			case 'h': // Help argument
				printf(USAGE);
				/* Leave program because we displayed help */
				exit(0);
				break;
			case '?': // Unknown argument
				printf(USAGE);
				break;	
		}
	}
}

/**
 * main
 * Purpose:
 *  Run the program
 *
 * Parameters:
 *  argc - argument count passed
 *  args - arguments passed
 * 
 * Returns:
 *  0 for success
 *  otherwise failure
 */
int main(int argc, char **args) {
	/* Initialize module variables */
	_global.total_people = 1;
	_global.max_wander_time = 10;
	_global.max_floor = 10;
	initialize_printing();
	/* Parse command arguments */
	find_options(argc, args);
	
	/* Prep modules for threads */
	_module.people_threads = malloc(sizeof(pthread_t)*_global.total_people);
	_module.people_time = malloc(sizeof(person_time)*_global.total_people);
	initialize_lift();
	sem_init(&_global.start_semaphore, 0, 1); //!< Initialize semaphore
	_global.uninitialized_people = _global.total_people;
	
	/* Temporarily allocate string to pass into later function */
	char *tmp_string = malloc(1);
	size_t size = 1;
	/* Parse total number of people */
	for (int i = 0; i < _global.total_people; ++i) {
		getline(&tmp_string, &size, stdin); //!< Get line from stdin
		parse_person_line(tmp_string, &_module.people_time[i]); //!< Parse it, and fill the people_time struct
		_module.people_time[i].id = i; //!< Give person an ID
	}
	free(tmp_string); //!< Free malloc'd string

	/* Start threads */
	pthread_create(&_module.lift_thread, NULL, start_lift_thread, NULL);
	for (int i = 0; i < _global.total_people; ++i) {
		pthread_create(&_module.people_threads[i], NULL, start_person_thread, &_module.people_time[i]);
	}

	/* Wait until lift thread finishes, it ensures every other thread is finished */
	pthread_join(_module.lift_thread, NULL);

	return 0;
}
