#include "../include/lift.h"
#include "../include/person.h"
#include "../include/global.h"

#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define VALID_OPTS "p:w:f:"

static struct {
	pthread_t lift_thread;
	pthread_t *people_threads;

	person_time *people_time;
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
	char opt;
	
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

int main(int argc, char **args) {
	/* Initialize module variables */
	_global.total_people = 1;
	_global.max_wander_time = 10;
	_global.max_floor = 10;
	/* Parse command arguments */
	find_options(argc, args);
	
	/* Prep modules for threads */
	_module.people_threads = malloc(sizeof(pthread_t)*_global.total_people);
	_module.people_time = malloc(sizeof(person_time)*_global.total_people);
	initialize_lift();
	
	char *tmp_string = malloc(1);
	size_t size = 1;
	for (int i = 0; i < _global.total_people; ++i) {
		getline(&tmp_string, &size, stdin);
		parse_person_line(tmp_string, &_module.people_time[i]);
		_module.people_time[i].id = i;
	}
	free(tmp_string);

	/* Start threads */
	pthread_create(&_module.lift_thread, NULL, start_lift_thread, NULL);
	for (int i = 0; i < _global.total_people; ++i) {
		pthread_create(&_module.people_threads[i], NULL, start_person_thread, &_module.people_time[i]);
	}

	/* set global start to 1 so all threads can execute */
	glob_start = 1;

	pthread_join(_module.lift_thread, NULL);

	return 0;
}
