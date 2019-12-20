#ifndef __GLOBAL_H_
#define __GLOBAL_H_

typedef enum _ERR_CODE {
	OK = 0,

	NOT_ENOUGH_MEMORY = -1,
	INVALID_COMMAND = -2,
	INVALID_ARGUMENTS = -3,
} ERR_CODE;

/* Struct that holds global members */
struct {
	long long total_size;
} _global;

#endif //__GLOBAL_H_
