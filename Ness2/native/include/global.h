#ifndef _GLOBAL_H_
#define _GLOBAL_H_

/**
 * List of possible error codes used in program
 */
typedef enum _ERR_CODE {
	OK = 0,
	INVALID_FILE = -1,
	INVALID_FILE_FORMAT = -2,
	PIPE_FAILURE = -3,
	FORK_FAILURE = -4,
	
	MEMORY_NOT_ALLOCATED = -10,

	ALL_ENDED = 1,

	GENERAL_FAILURE = -100,
} ERR_CODE;

#endif //_GLOBAL_H_

