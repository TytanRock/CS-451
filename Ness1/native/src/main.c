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
			case 'p':
				_module.header_info.pid_h = 1;
				/* If the optarg is invalid, this returns 0 which is handled later on in the program */
				_module.pid_num = atoi(optarg);
				break;
			case 's':
				_module.header_info.state_h = 1;
				break;
			case 't':
				_module.header_info.time_h = 1;
				break;
			case 'c':
				_module.header_info.cmd_h = 1;
				break;
			case 'v':
				_module.header_info.mem_h = 1;
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
	get_process_info(_module.pid_num, &(_module.header_info));

	char **print_string;
	char * tmp;
	print_string = &tmp;
	produce_pid_info(print_string);
	printf("%s", *print_string);
	
	/* The contract from produce_pid_info is we have to free the value */
	free(*print_string);

	return 0;
}
