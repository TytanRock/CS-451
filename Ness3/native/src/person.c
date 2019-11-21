#include "../include/person.h"
#include "../include/lift.h"

#include <unistd.h>

static struct {
	int current_floor;
} _module;

void start_person_thread(person_time *times, int time_count) {
	_module.current_floor = 0;

	for(int i = 0; i < time_count; ++i) {
		/* Push the button to let elevator know we're here */
		add_floor_stop(_module.current_floor);
		/* Busy wait until elevator is at our floor */
		while(get_floor_stop() != _module.current_floor) { }
		/* Busy wait until elevator is at the desired floor */
		while(get_floor_stop() != times[i].floor) { }
		/* Wait for specified time */
		sleep(times[i].wait);
	}
}
