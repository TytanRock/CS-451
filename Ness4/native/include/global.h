/**
 * Author: Cory Ness
 * Assignment Number: 4
 * Date of Submission: 12/19/2019
 * Name of this file: global.h
 * Description of program:
 *  Allocate memory based on user input, following different strategies
 *  Compact when needed
 *  And free memory based on user request
 * This project uses 1 coupon
 */

#ifndef __GLOBAL_H_
#define __GLOBAL_H_

typedef enum _ERR_CODE {
	OK = 0,

	NOT_ENOUGH_MEMORY = -1,
	INVALID_COMMAND = -2,
	INVALID_ARGUMENTS = -3,
	SEGMENT_DOESNT_EXIST = -4,
} ERR_CODE;

/* Struct that holds global members */
struct {
	long long total_size;
} _global;

#endif //__GLOBAL_H_
