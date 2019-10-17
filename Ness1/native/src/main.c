/**
 * Author: Cory
 * Assignment Number: 1
 * Date of Submission: ~
 * Name of this file: main.c
 * Description of the program:
 *  This program mimics the output of ps -ely for any specific pid
 *  It uses the /proc directory to find the process information that
 *  the user wants, specified by the 5 CLI parameters, p,s,t,v,c
 */

#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "../include/processes.h"

#define VALID_OPTS "p:stcv"

struct {
	unsigned int pid_num; //!< PID Number to take snapshot of
	
	struct _process_header header_info;
} _module;

/**
 * find_options
 * Purpose:
 *  Update module struct based on CLI parameters passed
 *
 * Parameters:
 *  argc - argument count as passed from main
 *  args - argument strings as passed from main
 */
void find_options(const int argc, char **args) {
	char opt;
	
	/* Get all arguments from the command line */
	while((opt = getopt(argc, args, VALID_OPTS)) != -1) {
		switch(opt) {
			case 'p': // PID argument
				_module.header_info.pid_h = 1;
				/* If the optarg is invalid, this returns 0 which is handled later on in the program */
				_module.pid_num = atoi(optarg);
				break;
			case 's': // State argument
				_module.header_info.state_h = 1;
				break;
			case 't': // Time argument
				_module.header_info.time_h = 1;
				break;
			case 'c': // CMD argument
				_module.header_info.cmd_h = 1;
				break;
			case 'v': // Mem argument
				_module.header_info.mem_h = 1;
				break;
			case '?': // Unknown argument
				if(optopt = 'p') {
					/* Set pid_num to an invalid pid number to allow program to exit */
					_module.pid_num = 0;
				}
				break;	
		}
	}
}

int main(int argc, char **args) {
	/* Initialize pid number to 1, following spec */
	_module.pid_num = 1;
	
	/* Parse command-line arguments */
	find_options(argc, args);
	
	/* Get process info from the pid number */
	ERR_CODE err = get_process_info(_module.pid_num, &(_module.header_info));
	
	/* If err is not OK, we need to tell user and close the program */
	if(err != OK) {
		/* Figure out what failed it and let uesr know */
		switch(err) {
			case PID_INVALID:
				fprintf(stderr, "PID is invalid\n");
				break;
			case STATFILE_NOTPRESENT:
				fprintf(stderr, "\"stat\" file for pid does not exit\n");
				break;	
			case STATMFILE_NOTPRESENT:
				fprintf(stderr, "\"statm\" file for pid does not exit\n");
				break;
			case CMDLINEFILE_NOTPRESENT:
				fprintf(stderr, "\"cmdline\" file for pid does not exit\n");
				break;
			case OTHER_ERR:
				fprintf(stderr, "Some error occured\n");
				break;
		}
		exit(-1); // Failed program
	}
	
	/* Create a pointer to a pointer */
	char **print_string;
	/* We need a char pointer to point to */
	char * tmp;
	/* Point to the char pointer */
	print_string = &tmp;
	
	/* Get all the information needed */
	produce_pid_info(print_string);
	
	/* Print it to the user */
	printf("%s", *print_string);
	
	/* The contract from produce_pid_info is we have to free the value */
	free(*print_string);

	exit(0); // Program succeeded
}
