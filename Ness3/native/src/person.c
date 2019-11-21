#include "../include/person.h"

static struct {
	person_times times;
} _module;

void start_person_thread(person_times *times) {
	_module.times = *times;
}
