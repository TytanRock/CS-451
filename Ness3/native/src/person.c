#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/global.h"
#include "../include/person.h"
#include "../include/lift.h"

void parse_person_line(char *str, person_time *ref) {
	sscanf(str, "%d", &ref->pair_count);
	
	ref->pairs = malloc(sizeof(ref->pairs) * ref->pair_count);

	for(int i = 0; i < ref->pair_count; ++i) {
		sscanf(str, "%d %d",
				&ref->pairs[i].floor,
				&ref->pairs[i].wait);
	}
}

void *start_person_thread(void *arg) {
	int current_floor = 0;

	person_time *time = (person_time *)arg;

	/* Busy wait until master thread is ready */
	while(!glob_start) ;

	for(int i = 0; i < time->pair_count; ++i) {
		/* Push the button to let elevator know we're here */
		add_floor_stop(current_floor);
		/* Busy wait until elevator is at our floor */
		while(get_floor_stop() != current_floor) { }
		/* Busy wait until elevator is at the desired floor */
		while(get_floor_stop() != time->pairs[i].floor) { }
		/* Wait for specified time */
		sleep(time->pairs[i].wait);
	}

	return NULL;
}
