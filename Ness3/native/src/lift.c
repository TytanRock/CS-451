#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#include "../include/global.h"

#define MUTEX_INIT 1

struct {
	unsigned int * floor_stops;
	unsigned int current_floor;

	sem_t button_mutex;
	sem_t floor_mutex;

	unsigned initialized : 1;
	unsigned going_up : 1;
}_module;

void add_floor_stop(unsigned int floor) {
	/* Ensure we're not pushing the button while the elevator is stopping */
	sem_wait(&_module.button_mutex);
	_module.floor_stops[floor] |= 1;
	sem_post(&_module.button_mutex);
}

int get_floor_stop() {
	/* Ensure we're not changing the floor state */
	sem_wait(&_module.floor_mutex);
	int ret = _module.current_floor;
	sem_post(&_module.floor_mutex);
	return ret;
}

void initialize_lift() {
	/* If we're not initialized, intialize it */
	if(!_module.initialized) {
		_module.floor_stops = malloc(_global.max_floor);
		_module.initialized = 1;
	/* Otherwise just reallocate the space so we can clean it later */
	} else {
		_module.floor_stops = realloc(_module.floor_stops, _global.max_floor);
	}
	/* Initialize other variables */
	sem_init(&_module.button_mutex, 0, MUTEX_INIT);
	sem_init(&_module.floor_mutex, 0, MUTEX_INIT);
}

void *start_lift_thread(void *arg) {
	/* Initialize variables for thread */
	int ended = 0;
	int stopped_at_floor = 0;

	/* Busy wait until master thread is ready */
	while(!glob_start);

	/* While there's people, do this */
	while(!ended) {
		/* Lock mutex and ensure nobody adds button states */
		sem_wait(&_module.button_mutex);
		/* Check if there's a reason to stop on this floor */
		if(_module.floor_stops[_module.current_floor] & 1) {
			_module.floor_stops[_module.current_floor] ^= 1;
			stopped_at_floor = 1;
			sleep(1); //!< Wait a second
		}
		sem_post(&_module.button_mutex); //!< Unlock mutex

		/* Check to see if we're at an end */
		sem_wait(&_module.floor_mutex);
		if(_module.current_floor == 0 || _module.current_floor == _global.max_floor) {
			_module.going_up ^= 1; //!< Invert direction
		}
		sem_post(&_module.floor_mutex);
		sleep(1); //!< Wait a second while we travel
		/* Move floor based on direction headed */
		if(_module.going_up) {
			_module.current_floor++;
		} else {
			_module.current_floor--;
		}
		/* Check if we've completed a cycle */
		if(_module.current_floor == 0 && !_module.going_up) {
			/* We've completed 2 whole cycles without a person */
			if(stopped_at_floor == 2) {
				ended = 1; //!< End this
			}
			/* We haven't seen a person */
			if(!stopped_at_floor) {
				sleep(_global.max_wander_time); //!< Wait for max wander time
				stopped_at_floor = 2;
			/* We have seen a person, nothing interesting */
			} else {
				stopped_at_floor = 0;
			}
		}
	}

	return NULL;
}

