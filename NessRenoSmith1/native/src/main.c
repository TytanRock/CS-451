#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "../include/processes.h"

#define VALID_OPTS "p:stcv"

struct {
	unsigned int pid_num; //!< PID Number to take snapshot of
	unsigned p_flag : 1;  //!< P Flag set
	unsigned s_flag : 1;  //!< S Flag set
	unsigned t_flag : 1;  //!< T Flag set
	unsigned c_flag : 1;  //!< C Flag set
	unsigned v_flag : 1;  //!< V Flag set
} _module;

void find_options(const int argc, char **args) {
	char opt;

	while((opt = getopt(argc, args, VALID_OPTS)) != -1) {
		switch(opt) {
			case 'p':
				_module.p_flag = 1;
				_module.pid_num = atoi(optarg);
				break;
			case 's':
				_module.s_flag = 1;
				break;
			case 't':
				_module.t_flag = 1;
				break;
			case 'c':
				_module.c_flag = 1;
				break;
			case 'v':
				_module.v_flag = 1;
				break;
		}
	}
}

int main(int argc, char **args) {
	find_options(argc, args);


	printf("vals are: %d\n", _module.pid_num);
	return 0;
}
