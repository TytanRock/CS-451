/**
 * Author: Cory Ness
 * Assignment Number: 3
 * Date of Submission: 12/5/2019
 * Name of this file: lift.c
 * Description of program:
 *  This program runs an elevator and n number of people
 *  The people get on and off the elevator and move to specified floors
 *  They wait for some number of seconds at each floor, and eventually leave the building
 * This project uses 1 coupon
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "../include/global.h"
#include "../include/printing.h"

#define MUTEX_INIT 1
/* Redefine printf to timed_print so we prepend the time */
#define printf timed_print

/* Struct of variables for this module */
static struct {
	unsigned int * floor_stops; //!< Array of floors the elevator needs to stop at
	unsigned int current_floor; //!< Current floor elevator is at

	sem_t button_mutex; //!< Semaphore for button pushing
	sem_t floor_mutex; //!< Semaphore for floor accessing

	unsigned initialized : 1; //!< Bool to keep track if module is initialized
	unsigned going_up : 1; //!< Bool to keep track if elevator is going up or down
}_module;

/**
 * add_floor_stop
 * Purpose: "Push the button" so elevator stops at a floor
 * Parameters:
 *  floor - Floor for elevator to stop at
 */
void add_floor_stop(unsigned int floor) {
	/* 
	 * Button mutex is locked while elevator is looking at button, 
	 * Ensures button isn't pressed immediately after elevator has cleared the button 
	 */
	sem_wait(&_module.button_mutex);
	_module.floor_stops[floor] = 1; //!< Add stop to array
	sem_post(&_module.button_mutex);
}

/**
 * get_floor_stop
 * Purpose: Get current floor for elevator
 * Returns: Floor elevator is at
 */
int get_floor_stop() {
	/* 
	 * Floor mutex is locked while elevator is moving between floors,
	 * Ensures floor location isn't looked at while elevator is between floors
	 */
	sem_wait(&_module.floor_mutex);
	int ret = _module.current_floor;
	sem_post(&_module.floor_mutex);
	return ret;
}

/**
 * initialize_lift
 * Purpose: Initialize lift module
 */
void initialize_lift() {
	/* If we're not initialized, intialize it */
	if(!_module.initialized) {
		_module.floor_stops = calloc(_global.max_floor + 1, sizeof(int));
		_module.initialized = 1;
	/* Otherwise just reallocate the space so we can clean it later */
	} else {
		_module.floor_stops = realloc(_module.floor_stops, _global.max_floor);
	}
	/* Initialize other variables */
	sem_init(&_module.button_mutex, 0, MUTEX_INIT);
	sem_init(&_module.floor_mutex, 0, MUTEX_INIT);
}

/**
 * start_lift_thread
 * Purpose: Run the elevator thread
 * Parameters:
 *  arg - Pointer to struct (unused)
 * Returns: void pointer to NULL
 */
void *start_lift_thread(void *arg) {
	/* Initialize variables for thread */
	int ended = 0;
	int stopped_at_floor = 0;
	_module.going_up = 1;

	/* Busy wait until master thread is ready */
	unsigned int unitialized_people = 0;
	do {
		sem_wait(&_global.start_semaphore);
		unitialized_people = _global.uninitialized_people;
		sem_post(&_global.start_semaphore);
	}
	while(unitialized_people != 0);
	usleep(10000); //!< Wait to ensure people are all initialized before lift
	printf("Elevator:\t\t\tAt %d\n", 0);


	/* While there's people, do this */
	while(!ended) {
		/* Lock mutex and ensure nobody adds button states */
		sem_wait(&_module.button_mutex);

		/* Check if there's a reason to stop on this floor */
		if(_module.floor_stops[_module.current_floor]) {
			_module.floor_stops[_module.current_floor] = 0;
			if(_module.current_floor != 0) { 
				stopped_at_floor = 1;
			}
			printf("Elevator:\t\t\tStopping at this floor\n");
			sleep(1); //!< Wait a second
		}
		sem_post(&_module.button_mutex); //!< Unlock mutex

		/* Check to see if we're at an end */
		if(_module.current_floor == 0) {
			_module.going_up = 1; //!< We're at bottom, let's go up
		}else if(_module.current_floor == _global.max_floor) {
			_module.going_up = 0; //!< We're at top, let's go down
		}
		sem_wait(&_module.floor_mutex);
		sleep(1); //!< Wait a second while we travel
		/* Move floor based on direction headed */
		if(_module.going_up) {
			_module.current_floor++;
		} else {
			_module.current_floor--;
		}
		sem_post(&_module.floor_mutex);

		printf("Elevator:\t\t\tAt %d\n", _module.current_floor);
		/* Check if we've completed a cycle */
		if(_module.current_floor == 0 && !_module.going_up) {
			/* We've completed 2 whole cycles without a person */
			if(stopped_at_floor == 2) {
				ended = 1; //!< End this
				printf("Elevator:\t\t\tNobody was picked up, exiting program\n");
			/* We haven't seen a person */
			} else if(!stopped_at_floor) {
				printf("Elevator:\t\t\tHaven't seen anybody this trip, waiting for %d seconds\n", _global.max_wander_time);
				sleep(_global.max_wander_time - 1); //!< Wait for max wander time minus 1, since we already wait 1 second
				stopped_at_floor = 2;
				/* We have seen a person, nothing interesting */
			} else {
				stopped_at_floor = 0;
				printf("Elevator:\t\t\tCycle complete, I saw at least 1 person\n");
			}
		}
	}

	return NULL;
}

