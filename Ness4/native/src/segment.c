#include "../include/segment.h"
#include "../include/global.h"

#include <stdlib.h>

static struct {
	segment_t *memories;
	int memory_length;
} _module;

void initialize_memory(int size) {
	_module.memories = malloc(sizeof(segment_t) * 1);
	_module.memories[0].address = 0;
	_module.memories[0].size = size;
	_module.memories[0].free = 1;
	_module.memory_length = 1;
}

ERR_CODE allocate_memory(char *name, int size, strategy strat) {
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
		default: break;
	}

	return OK;
}

