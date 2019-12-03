/**
 * Cory Ness
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

#define VALID_OPTS "p:w:f:"

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
			case '?': // Unknown argument
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

	/* set global start to 1 so all threads can execute */
	glob_start = 1;

	/* Wait until lift thread finishes, it ensures every other thread is finished */
	pthread_join(_module.lift_thread, NULL);

	return 0;
}
