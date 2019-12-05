/**
 * Author: Cory Ness
 * Assignment Number: 3
 * Date of Submission: 12/5/2019
 * Name of this file: person.c
 * Description of program:
 *  This program runs an elevator and n number of people
 *  The people get on and off the elevator and move to specified floors
 *  They wait for some number of seconds at each floor, and eventually leave the building
 * This project uses 1 coupon
 */


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/global.h"
#include "../include/person.h"
#include "../include/lift.h"
#include "../include/printing.h"

/* Redefine printf to timed_print so we have timing */
#define printf timed_print

/**
 * parse_person_line
 * Purpose: Parse string into a set of person_time's
 * Parameters:
 *  str - String to parse
 *  ref - Struct to fill with data
 */
void parse_person_line(char *str, person_time *ref) {
	/* Get number of pairs */
	sscanf(str, "%d", &ref->pair_count);
	/* Allocate enough space for each pair */
	ref->pairs = malloc(sizeof(ref->pairs) * ref->pair_count);
	str = strstr(str, " ") + 1; //!< Move pointer to next location

	/* Get the specified number of pairs */
	for(int i = 0; i < ref->pair_count; ++i) {
		/* Scanf the pair */
		sscanf(str, "%d %d",
				&ref->pairs[i].floor,
				&ref->pairs[i].wait);
		/* Move the pointer to the next set of pairs */
		str = strstr(str, " ") + 1;
		str = strstr(str, " ") + 1;
	}
}

/**
 * start_person_thread
 * Purpose: Start the thread for a person
 * Parameters:
 *  arg - person_time struct with everything the person needs
 * Returns: void* that's always NULL
 */
void *start_person_thread(void *arg) {
	/* Initialize function variables */
	int current_floor = 0;

	person_time *time = (person_time *)arg; //!< Cast arg into person_time in order to use it

	/* Decrement uninitialized people */
	sem_wait(&_global.start_semaphore);
	_global.uninitialized_people --;
	sem_post(&_global.start_semaphore);	

	/* For every pair, we need to do the following sequence */
	for(int i = 0; i < time->pair_count; ++i) {
		/* Print where we are and where we're going */
		printf("Person %d:\t%d-->%d\n", time->id, current_floor, time->pairs[i].floor);
		/* Push the button to let elevator know we're here */
		add_floor_stop(current_floor);
		/* Let user know button was pushed */
		printf("Person %d:\tI pushed the button for floor %d\n", time->id, current_floor);
		/* Busy wait until elevator is at our floor */
		while(get_floor_stop() != current_floor) { }
		/* Let user know we got onto the elevator */
		printf("Person %d:\tOn elevator\n", time->id);
		/* Push button for where to go */
		add_floor_stop(time->pairs[i].floor);
		/* Let user know the elevator will stop at the next floor */
		printf("Person %d:\tI pushed the button for floor %d\n", time->id, time->pairs[i].floor);
		/* Busy wait until elevator is at the desired floor */
		while(get_floor_stop() != time->pairs[i].floor) { }
		/* Let user know we're off the elevator */
		printf("Person %d:\tOff elevator, waiting for %ds\n", time->id, time->pairs[i].wait);
		current_floor = time->pairs[i].floor; //!< Move current floor to where we're now at
		/* Wait for specified time */
		sleep(time->pairs[i].wait);
	}
	
	/* Let user know we're leaving the building */
	printf("Person %d:\tI'm leaving\n", time->id);

	/* Free malloc'd pair array */
	free(time->pairs);

	return NULL;
}
