/**
 * Author: Cory Ness
 * Assignment Number: 4
 * Date of Submission: 12/19/2019
 * Name of this file: segment.c
 * Description of program:
 *  Allocate memory based on user input, following different strategies
 *  Compact when needed
 *  And free memory based on user request
 * This project uses 1 coupon
 */

#include "../include/segment.h"
#include "../include/global.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Keep a struct holding all the relevant information for this module
 */
static struct {
	segment_t *memories; // Array of segments
	int memory_length; // Length of the array
} _module;

/**
 * compare
 * Purpose: Compare two segments
 * Parameters:
 *  a - First segment
 *  b - Second segment
 * Returns - 0 if equal, + if a>b, - if a<b
 */
int compare(const void *a, const void* b) {
	return ((segment_t*)a)->address - ((segment_t*)b)->address; // Just subtract address to get first value
}

/**
 * combine_adjacent_free
 * Purpose: Combine adjacent free segments
 */
void combine_adjacent_free() {
	/* Keep combining until memory length doesn't change */
	int old_length; // Use to figure out if we've gotten smaller
	do {
		old_length = _module.memory_length; // Set this variable to current length
		/* First, sort all the memory so that we can go down the line */
		qsort(_module.memories, _module.memory_length, sizeof(segment_t), compare);
		
		/* Create an array of flags to determine if the segment still exists */
		int *exists;
		exists = calloc(sizeof(int), _module.memory_length);

		/* We look at every segment in the array */
		for(int i = 1; i < _module.memory_length; ++i) {		
			segment_t *first = &_module.memories[i-1];
			segment_t *second = &_module.memories[i];

			/* If they are both free and adjacent, combine them */
			if(first->free && second->free && second->address == first->address + first->size) {
				/* Modify second size since it's before first segment */
				first->size += second->size;
				/* Clear first size */
				second->size = 0;
				second->address = first->address;
				exists[i] = 1;
			}
			
		}

		/* Go through the exists column and remove everything that doesn't exist anymore */
		for(int i = _module.memory_length - 1; i >= 0; --i) {
			/* If segment doesn't exist, replace it with end of array */
			if(exists[i] == 1) {
				_module.memories[i] = _module.memories[_module.memory_length - 1];
				--_module.memory_length;
			}
		}

		/* All unused segments are gone, realloc to the correct size */
		_module.memories = realloc(_module.memories, _module.memory_length * sizeof(segment_t));
		free(exists);
	} while (_module.memory_length != old_length); // Do this again if the segment array changed
}

/**
 * initialize_memory
 * Purpose: Initialize this module
 */
void initialize_memory() {
	/* Initialize all module variables */
	_module.memories = malloc(sizeof(segment_t) * 1);
	_module.memories[0].address = 0;
	_module.memories[0].size = _global.total_size;
	_module.memories[0].free = 1;
	_module.memory_length = 1;
}

/**
 * allocate_memory
 * Purpose: Create a new segment with specified size, following the specified strategy
 * Parameters:
 *  name - name of segment
 *  size - size of segment
 *  strat - strategy used to allocate memory
 * Returns - Error code based on succss of function
 */
ERR_CODE allocate_memory(char *name, long long size, strategy strat) {
	/* Switch allocation scheme based on strategy */
	switch(strat) {
		case worst: ; // Cannot declare immediately after a label
			int worst_memory = 0;
			/* Find worst spot for memory allocation */
			for(int i = 0; i < _module.memory_length; ++i) {
				/* If this segment is worse, use it */
				if(_module.memories[i].free && _module.memories[i].size > _module.memories[worst_memory].size) {
					worst_memory = i;
				}
			}
			/* If there is no valid entry, let user know there was no space */
			if(_module.memories[worst_memory].size < size || _module.memories[worst_memory].free == 0) {
				/* Fail this call */
				return NOT_ENOUGH_MEMORY;
			}

			/* Go through and create a new segment, modifying everything needed */
			++_module.memory_length;
			_module.memories = realloc(_module.memories, sizeof(segment_t) * _module.memory_length);
			_module.memories[_module.memory_length - 1].size = size;
			_module.memories[_module.memory_length - 1].address = _module.memories[worst_memory].address;
			_module.memories[_module.memory_length - 1].free = 0;
			_module.memories[_module.memory_length - 1].name = name;
			_module.memories[worst_memory].address += size;
			_module.memories[worst_memory].size -= size;

			/* If we used up the rest of the space, we need to remove this segment */
			if(_module.memories[worst_memory].size == 0) {
				/* Remove it from the list */
				_module.memories[worst_memory] = _module.memories[_module.memory_length - 1];
				--_module.memory_length;
				
				/* This actually removes the memory */
				_module.memories = realloc(_module.memories, sizeof(segment_t) * _module.memory_length);
			}
			break;
		case first: ; // Cannot declare immediately after a label
			int success = 0;
			/* Find the first spot for memory allocation */
			for(int i = 0; i < _module.memory_length; ++i) {
				/* If this fits, use it */
				if(_module.memories[i].free && _module.memories[i].size >= size) {
					/* Go through and create a new segment, modifying everything needed */
					++_module.memory_length;
					_module.memories = realloc(_module.memories, sizeof(segment_t) * _module.memory_length);
					_module.memories[_module.memory_length - 1].size = size;
					_module.memories[_module.memory_length - 1].address = _module.memories[i].address;
					_module.memories[_module.memory_length - 1].free = 0;
					_module.memories[_module.memory_length - 1].name = name;
					_module.memories[i].address += size;
					_module.memories[i].size -= size;
					success = 1;

					/* If we used up the rest of the space, we need to remove the segment */
					if(_module.memories[i].size == 0) {
						_module.memories[i] = _module.memories[_module.memory_length - 1];
						--_module.memory_length;

						/* Actually removes it from memory */
						_module.memories = realloc(_module.memories, sizeof(segment_t) * _module.memory_length);
					}
					break;
				}
			}
			/* If we never ran this, we need to let user know there was no space */
			if(!success) {
				/* Fail this call */
				return NOT_ENOUGH_MEMORY;
			}
			break;
		case best: ;
			int best_memory;
			best_memory = 0;
			/* Find the best memory location for this */
			for(int i = 0; i < _module.memory_length; ++i) {
				/* If this is a better location for memory, use it */
				if((_module.memories[i].free && 
						_module.memories[i].size < _module.memories[best_memory].size &&
						_module.memories[i].size > size) ||
						(_module.memories[best_memory].free == 0 ||
						 _module.memories[best_memory].size < size)) {
					best_memory = i;
				}
			}
			/* If there is no valid memory slot, we need to fail this */
			if(_module.memories[best_memory].size < size ||
				_module.memories[best_memory].free == 0) {
				/* Fail this call */
				return NOT_ENOUGH_MEMORY;
			}

			/* Go through and create a new segment, modifying everything needed */
			++_module.memory_length;
			_module.memories = realloc(_module.memories, sizeof(segment_t) * _module.memory_length);
			_module.memories[_module.memory_length - 1].size = size;
			_module.memories[_module.memory_length - 1].address = _module.memories[best_memory].address;
			_module.memories[_module.memory_length - 1].free = 0;
			_module.memories[_module.memory_length - 1].name = name;
			_module.memories[best_memory].address += size;
			_module.memories[best_memory].size -= size;

			/* If we used up the rest of the psace, we need to remove the segment */
			if(_module.memories[best_memory].size == 0) {
				_module.memories[best_memory] = _module.memories[_module.memory_length - 1];
				--_module.memory_length;

				/* This actually removes the segment */
				_module.memories = realloc(_module.memories, sizeof(segment_t) * _module.memory_length);
			}
			break;

		default: break; // Default condition doesn't do anything
	}

	return OK; /* We made it, return OK */
}

/**
 * deallocate_memory
 * Purpose: Remove a segment from the arrays
 * Parameters:
 *  name - name of segment to remove
 * Return - Error code based the status of the function
 */
ERR_CODE deallocate_memory(char *name) {
	segment_t *memory_to_clear = NULL;
	/* Go through every memory segment and find the same name */
	for(int i = 0; i < _module.memory_length; ++i) {
		/* Compare current segment for name */
		if(_module.memories[i].free == 0 && strcmp(_module.memories[i].name, name) == 0) {
			memory_to_clear = &_module.memories[i];
			/* Free malloc'd name */
			free(_module.memories[i].name);
		}
	}
	
	/* If memory doesn't exist, let user know */
	if(memory_to_clear == NULL) {
		return SEGMENT_DOESNT_EXIST;
	}

	/* Set memory free flag */
	memory_to_clear->free = 1;

	/* Combine adjacent free memory segments */
	combine_adjacent_free();

	return OK; // Good to go, returns OK
}

/**
 * compact_memory
 * Purpose: Compact the memory, pushing all the memory to the top
 * Returns - Error code based on status of the function
 */
ERR_CODE compact_memory() {
	/* First, sort all the memory so that we can go down the line */
	qsort(_module.memories, _module.memory_length, sizeof(segment_t), compare);

	/* Create a temporary variable that keeps track of the last available address */
	long long last_address = 0;
	long long last_free = _global.total_size;

	/* Go down the line finding every non-free memory and pushing it to the top */
	for(int i = 0; i < _module.memory_length; ++i) {
		/* Check to see if it's taken up */
		if(_module.memories[i].free == 0) {
			/* Push it to the top */
			_module.memories[i].address = last_address;
			last_address += _module.memories[i].size;
		} else {
			/* Push free space to the bottom */
			last_free -= _module.memories[i].size;
			_module.memories[i].address = last_free;
		}
	}
	/* Take care of all the combined segments now */
	combine_adjacent_free();

	return OK; // Good to go, returns OK
}

/**
 * print_memory
 * Purpose: Print all the segments out in address order
 * Returns - Error code based on status of function
 */
ERR_CODE print_memory() {
	/* First make sure everything is sorted in address order */
	qsort(_module.memories, _module.memory_length, sizeof(segment_t), compare);

	/* Then print address, size, name of every segment */
	for(int i = 0; i < _module.memory_length; ++i) {
		/* Print following this format */
		printf("Address: %10lld Size: %10lld Name: %s\n", 
				_module.memories[i].address,
				_module.memories[i].size,
				_module.memories[i].free ? "Free Memory" :
				_module.memories[i].name);
	}

	return OK; // Good to go, return OK
}

