#include "../include/segment.h"
#include "../include/global.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct {
	segment_t *memories;
	int memory_length;
} _module;

void combine_adjacent_free() {
	/* O(n^2) algorithm */
	
	/* Create an array of flags to determine if the segment still exists */
	int *exists;
	exists = calloc(sizeof(int), _module.memory_length);

	/* We look at every segment in the array */
	for(int i = 0; i < _module.memory_length; ++i) {
		/* We compare it against every other segment in the array */
		for(int j = 0; j < _module.memory_length; ++j) {
			segment_t *first = &_module.memories[i];
			segment_t *second = &_module.memories[j];

			/* If they are both free and adjacent, combine them */
			if(first->free && second->free && first->address == second->address + second->size) {
				/* Modify second size since it's before first segment */
				second->size = second->size + first->size;
				/* Clear first size */
				first->size = 0;
				exists[i] = 0;
				/* Free memory allocated in first */
				free(first->name);
			} else {
				exists[i] = 1;
			}
		}
	}

	/* Go through the exists column and remove everything that doesn't exist anymore */
	for(int i = _module.memory_length - 1; i >= 0; ++i) {
		/* If segment doesn't exist, replace it with end of array */
		if(!exists[i]) {
			_module.memories[i] = _module.memories[_module.memory_length - 1];
			--_module.memory_length;
		}
	}

	/* All unused segments are gone, realloc to the correct size */
	_module.memories = realloc(_module.memories, _module.memory_length * sizeof(segment_t));
}

int compare(const void *a, const void* b) {
	return ((segment_t*)a)->address - ((segment_t*)b)->address;
}

void initialize_memory(long long size) {
	_module.memories = malloc(sizeof(segment_t) * 1);
	_module.memories[0].address = 0;
	_module.memories[0].size = size;
	_module.memories[0].free = 1;
	_module.memory_length = 1;
}

ERR_CODE allocate_memory(char *name, long long size, strategy strat) {
	switch(strat) {
		case worst:
			;
			segment_t *worst_memory;
			worst_memory = _module.memories;
			for(int i = 0; i < _module.memory_length; ++i) {
				if(_module.memories[i].free && _module.memories[i].size > worst_memory->size) {
					worst_memory = &(_module.memories[i]);
				}
			}
			if(worst_memory->size < size) {
				/* Fail this call */
				return NOT_ENOUGH_MEMORY;
			}
			++_module.memory_length;
			_module.memories = realloc(_module.memories, sizeof(segment_t) * _module.memory_length);
			_module.memories[_module.memory_length - 1].size = size;
			_module.memories[_module.memory_length - 1].address = worst_memory->address + size;
			_module.memories[_module.memory_length - 1].free = 0;
			_module.memories[_module.memory_length - 1].name = name;
			break;
		case first:
			;
			int success = 0;
			for(int i = 0; i < _module.memory_length; ++i) {
				if(_module.memories[i].free && _module.memories[i].size > size) {
					++_module.memory_length;
					_module.memories = realloc(_module.memories, sizeof(segment_t) * _module.memory_length);
					_module.memories[_module.memory_length - 1].size = size;
					_module.memories[_module.memory_length - 1].address = _module.memories[i].address + size;
					_module.memories[_module.memory_length - 1].free = 0;
					_module.memories[_module.memory_length - 1].name = name;
					success = 1;
					break;
				}
			}
			if(!success) {
				return NOT_ENOUGH_MEMORY;
			}
			break;
		case best:
			;
			int best_memory;
			best_memory = 0;
			for(int i = 0; i < _module.memory_length; ++i) {
				if((_module.memories[i].free && 
					_module.memories[i].size < _module.memories[best_memory].size &&
					_module.memories[i].size > size) ||
					_module.memories[best_memory].free == 0) {
					best_memory = i;
				}
			}
			if(_module.memories[best_memory].size < size) {
				/* Fail this call */
				return NOT_ENOUGH_MEMORY;
			}
			++_module.memory_length;
			_module.memories = realloc(_module.memories, sizeof(segment_t) * _module.memory_length);
			_module.memories[_module.memory_length - 1].size = size;
			_module.memories[_module.memory_length - 1].address = _module.memories[best_memory].address;
			_module.memories[_module.memory_length - 1].free = 0;
			_module.memories[_module.memory_length - 1].name = name;
			_module.memories[best_memory].address += size;
			_module.memories[best_memory].size -= size;
			break;

		default: break;
	}

	return OK;
}

ERR_CODE deallocate_memory(char *name) {
	segment_t *memory_to_clear = NULL;
	/* Go through every memory segment and find the same name */
	for(int i = 0; i < _module.memory_length; ++i) {
		/* Compare current segment for name */
		if(strcmp(_module.memories[i].name, name) == 0) {
			memory_to_clear = &_module.memories[i];
		}
	}
	
	if(memory_to_clear == NULL) {
		return -1;
	}

	/* Set memory free flag */
	memory_to_clear->free = 1;

	/* Combine adjacent free memory segments */
	combine_adjacent_free();

	return OK;
}

void print_memory() {
	/* First make sure everything is sorted in address order */
	qsort(_module.memories, _module.memory_length, sizeof(segment_t), compare);

	/* Then print address, size, name of every segment */
	for(int i = 0; i < _module.memory_length; ++i) {
		printf("Address: %10lld Size: %10lld Name: %s\n", 
				_module.memories[i].address,
				_module.memories[i].size,
				_module.memories[i].free ? "Free Memory" :
				_module.memories[i].name);
	}
}

