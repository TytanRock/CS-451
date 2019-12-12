#include <stdio.h>
#include <stdlib.h>

#define USAGE "Usage: memory <size>\n"

static struct {
	long long total_size;
} _module;

int main(int argc, char **args) {
	
	if(argc != 2) {
		printf(USAGE);
		return 0;
	}

	_module.total_size = atoi(args[1]);
	if(_module.total_size <= 0) {
		printf("Invalid parameter passed\n");
		printf(USAGE);
	}
	
	printf("Val is: %lld\n", _module.total_size);

	return 0;
}
