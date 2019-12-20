#ifndef __SEGMENT_H_
#define __SEGMENT_H_

#include "global.h"

typedef struct _segment_t {
	long long size;
	long long address;
	char * name;

	unsigned free : 1;
}segment_t;

typedef enum _strategy {
	worst = 'W',
	best = 'B',
	first = 'F',
}strategy;

ERR_CODE allocate_memory(char *name, long long size, strategy strat);
ERR_CODE deallocate_memory(char *name);
ERR_CODE compact_memory();
ERR_CODE print_memory();
void initialize_memory();

#endif //__SEGMENT_H_
