#ifndef __SEGMENT_H_
#define __SEGMENT_H_

typedef struct _segment_t {
	long long size;
	long long address;
	char * name;

	unsigned free : 1;
}segment_t;

typedef enum _strategy {
	worst,
	best,
	first,
}strategy;

#endif //__SEGMENT_H_
