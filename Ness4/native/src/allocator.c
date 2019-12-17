#include "../include/segment.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE "Usage: allocator <size>\n"

static struct {
	long long total_size;
} _module;

void execute_command(char *command, size_t strlen) {
	if(strncmp("RQ", command, 2) == 0) {
		/* Requesting memory, so let's parse for name and size */
		char *name;
		long long sector;
		char strat;
		
		/* Allocate memory that is "strlen" for name */
		/* That way it allocates at least as much space as needed for the name */
		name = malloc(sizeof(char) * strlen);

		/* Parse name and size */
		sscanf(command, "%*s %s %lld %c", name, &sector, &strat);
		
		/* Add sector */
		allocate_memory(name, sector, (strategy)strat);
	}
	if(strncmp("RL", command, 2) == 0) {
		/* Releasing memory, let's get the name */
		char *name;
		name = malloc(sizeof(char) * strlen);

		sscanf(command, "%*s %s", name);

		/* Remove segment */
		deallocate_memory(name);
	}
	if(strncmp("STAT", command, 4) == 0) {
		/* Print everything */
		print_memory();
	}
}

int main(int argc, char **args) {
	
	/* Check to ensure correct number of arguments */	
	if(argc != 2) {
		printf(USAGE);
		return 0;
	}
	
	/* Assume parameter is successful */
	_module.total_size = atoi(args[1]);
	if(_module.total_size <= 0) {
		/* If it isn't, yell at user */
		printf("Invalid parameter passed\n");
		printf(USAGE);
	}
	
	/* Initialize variables for user input */
	char *line;
	line = NULL;
	size_t line_length;

	initialize_memory(_module.total_size);

	/* Infinitely prompt user until they interrupt it out */
	while(1) {
		/* Prompt User */
		printf("allocator >");
		/* Read input */
		getline(&line, &line_length, stdin);
		/* Execute user's input */
		execute_command(line, line_length);
		/* Do this again */
	}

	return 0;
}
