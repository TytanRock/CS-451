/**
 * Author: Cory Ness
 * Assignment Number: 4
 * Date of Submission: 12/19/2019
 * Name of this file: allocator.c
 * Description of program:
 *  Allocate memory based on user input, following different strategies
 *  Compact when needed
 *  And free memory based on user request
 * This project uses 1 coupon
 */

#include "../include/segment.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE "Usage: allocator <size>\n"
#define PARAMATER_USAGE "Invalid parameter, parameters are: \n\
	RQ <name> <size> <strategy>\n\
	RL <name>\n\
	C\n\
	STAT\n\
	X\n"
#define COMMAND_USAGE "Invalid command, commands are: RQ, RL, C, STAT, X\n"
#define NO_MEMORY "Unable to allocate space, no continuous segment has enough memory. Consider compacting or releasing some memory\n"
#define DOESNT_EXIST "Name of memory doesn't exist, use STATUS to see names of segments that do exist\n" 

/**
 * execute_command
 * Purpose: Execute the command based on the user input
 * Parameters:
 *  command - String of command to execute
 *  strlen - size of string to execute
 * Returns - Error code from the executed command
 */
ERR_CODE execute_command(char *command, size_t strlen) {
	ERR_CODE ret = INVALID_COMMAND; //Return variable
	
	/* Check to see if it's a request */
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
		ret = allocate_memory(name, sector, (strategy)strat);
	}
	/* Check to see if it's a release */
	if(strncmp("RL", command, 2) == 0) {
		/* Releasing memory, let's get the name */
		char *name;
		name = malloc(sizeof(char) * strlen);

		/* Parse command to get name */
		sscanf(command, "%*s %s", name);

		/* Remove segment */
		ret = deallocate_memory(name);
	}
	/* Check to see if it's a compact */
	if(strncmp("C", command, 1) == 0) {
		/* Compacting memory */
		ret = compact_memory();
	}
	/* Check to see if it's a print information */
	if(strncmp("STAT", command, 4) == 0) {
		/* Print everything */
		ret = print_memory();
	}
	/* Check to see if it's an exit */
	if(strncmp("X", command, 1) == 0) {
		/* Exit */
		exit(EXIT_SUCCESS);
	}

	return ret; // Return the retval
}

/**
 * main
 * Purpose: Run the program
 * Parameters:
 *  argc - argument count
 *  args - Array of argument strings
 * Returns status of program
 */
int main(int argc, char **args) {
	
	/* Check to ensure correct number of arguments */	
	if(argc != 2) {
		printf(USAGE);
		return 0;
	}
	
	/* Assume parameter is successful */
	_global.total_size = atoi(args[1]);
	if(_global.total_size <= 0) {
		/* If it isn't, yell at user */
		printf("Invalid parameter passed\n");
		printf(USAGE);
	}
	
	/* Initialize variables for user input */
	char *line;
	line = NULL;
	size_t line_length;

	initialize_memory();

	/* Infinitely prompt user until they interrupt it out */
	while(1) {
		/* Prompt User */
		printf("allocator >");
		/* Read input */
		getline(&line, &line_length, stdin);
		/* Execute user's input */
		ERR_CODE message = execute_command(line, line_length);
		
		/* Switch what to report back based on the return code */
		switch(message) {
			case OK: break; // No issue
			
			case INVALID_COMMAND: // Let user know the command is invalid
				printf(COMMAND_USAGE);
				break;
			case INVALID_ARGUMENTS: // Let user know that the parameter usage is invalid
				printf(PARAMATER_USAGE);
				break;
			case NOT_ENOUGH_MEMORY: // Let user know there isn't enough memory
				printf(NO_MEMORY);
				break;
			case SEGMENT_DOESNT_EXIST: // Let user know the segment name doesn't exist
				printf(DOESNT_EXIST);
				break;
		}
		/* Do this again */
	}

	return 0;
}
