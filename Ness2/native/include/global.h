#ifndef _GLOBAL_H_
#define _GLOBAL_H_

typedef enum _ERR_CODE {
	OK = 0,
	INVALID_FILE = -1,
	INVALID_FILE_FORMAT = -2,
	PIPE_FAILURE = -3,
	FORK_FAILURE = -4,

	GENERAL_FAILURE = -100,
} ERR_CODE;

#endif //_GLOBAL_H_

