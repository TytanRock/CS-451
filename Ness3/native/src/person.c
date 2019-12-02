#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/global.h"
#include "../include/person.h"
#include "../include/lift.h"
#include "../include/printing.h"

#define printf timed_print

void parse_person_line(char *str, person_time *ref) {
	sscanf(str, "%d", &ref->pair_count);
	ref->pairs = malloc(sizeof(ref->pairs) * ref->pair_count);
	str = strstr(str, " ") + 1;
	for(int i = 0; i < ref->pair_count; ++i) {
		sscanf(str, "%d %d",
				&ref->pairs[i].floor,
				&ref->pairs[i].wait);
		str = strstr(str, " ") + 1;
		str = strstr(str, " ") + 1;
	}
}

void *start_person_thread(void *arg) {
	int current_floor = 0;

	person_time *time = (person_time *)arg;

	/* Busy wait until master thread is ready */
	while(!glob_start) ;

	for(int i = 0; i < time->pair_count; ++i) {
		printf("Person %d:\t%d-->%d\n", time->id, current_floor, time->pairs[i].floor);
		/* Push the button to let elevator know we're here */
		add_floor_stop(current_floor);
		printf("Person %d:\tI pushed the button for floor %d\n", time->id, current_floor);
		/* Busy wait until elevator is at our floor */
		while(get_floor_stop() != current_floor) { }
		printf("Person %d:\tOn elevator\n", time->id);
		/* Push button for where to go */
		add_floor_stop(time->pairs[i].floor);
		printf("Person %d:\tI pushed the button for floor %d\n", time->id, time->pairs[i].floor);
		/* Busy wait until elevator is at the desired floor */
		while(get_floor_stop() != time->pairs[i].floor) { }
		printf("Person %d:\tOff elevator, waiting for %ds\n", time->id, time->pairs[i].wait);
		current_floor = time->pairs[i].floor;
		/* Wait for specified time */
		sleep(time->pairs[i].wait);
	}

	printf("Person %d:\tI'm leaving\n", time->id);

	return NULL;
}
