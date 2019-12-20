/**
 * Author: Cory Ness
 * Assignment Number: 4
 * Date of Submission: 12/19/2019
 * Name of this file: segment.h
 * Description of program:
 *  Allocate memory based on user input, following different strategies
 *  Compact when needed
 *  And free memory based on user request
 * This project uses 1 coupon
 */

#ifndef __SEGMENT_H_
#define __SEGMENT_H_

#include "global.h"

/* Struct holding all the relevant information for a segment */
typedef struct _segment_t {
	long long size; // Size of memory segment
	long long address; // Start address of memory segmnet
	char * name; // Name of segment

	unsigned free : 1; // Boolean that checks if segment is free
}segment_t;

/* Enum holding different strategies */
typedef enum _strategy {
	worst = 'W', // Worst fit
	best = 'B', // Best fit
	first = 'F', // First fit
}strategy;


/**
 * allocate_memory
 * Purpose: Create a new segment with specified size, following the specified strategy
 * Parameters:
 *  name - name of segment
 *  size - size of segment
 *  strat - strategy used to allocate memory
 * Returns - Error code based on succss of function
 */
ERR_CODE allocate_memory(char *name, long long size, strategy strat);

/**
 * deallocate_memory
 * Purpose: Remove a segment from the arrays
 * Parameters:
 *  name - name of segment to remove
 * Return - Error code based the status of the function
 */
ERR_CODE deallocate_memory(char *name);

/**
 * compact_memory
 * Purpose: Compact the memory, pushing all the memory to the top
 * Returns - Error code based on status of the function
 */
ERR_CODE compact_memory();

/**
 * print_memory
 * Purpose: Print all the segments out in address order
 * Returns - Error code based on status of function
 */
ERR_CODE print_memory();

/**
 * initialize_memory
 * Purpose: Initialize this module
 */
void initialize_memory();

#endif //__SEGMENT_H_
