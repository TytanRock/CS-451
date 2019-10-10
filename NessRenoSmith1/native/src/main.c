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

	while((opt = getopt(argc, args, VALID_OPTS)) != -1) {
		switch(opt) {
			case 'p':
				_module.header_info.pid_h = 1;
				_module.pid_num = atoi(optarg);
				break;
			case 's':
				//_module.s_flag = 1;
				break;
			case 't':
				//_module.t_flag = 1;
				break;
			case 'c':
				//_module.c_flag = 1;
				break;
			case 'v':
				//_module.v_flag = 1;
				break;
		}
	}
}

int main(int argc, char **args) {
	find_options(argc, args);
	
	get_process_info(_module.pid_num);

	printf("vals are: %d\n", _module.pid_num);
	return 0;
}
