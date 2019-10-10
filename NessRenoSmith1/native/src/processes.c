#include "../include/processes.h"
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

struct {
	struct {
		unsigned pid_h : 1;	//!< Append the PID column
		unsigned tty_h : 1;	//!< Append the TTY column
		unsigned time_h : 1;	//!< Append the TIME column
		unsigned cmd_h : 1;	//!< Append the CMD column
	}header_info; //!< Struct that holds header information
	
	int pid;		//!< PID Number
	char * tty;		//!< TTY owner
	unsigned long time;	//!< Time alive
	char * cmd;		//!< CMD that called it
}_module; //!< Module-specific information


void fill_module(struct dirent * dir) {

}

/**
 * get_process_info
 * Purpose:
 *  Checks if process exists and calls the fill function if it does
 *
 * Parameters:
 *  pid - ID number of process to get information from
 *
 * Returns:
 *  0 for success, -1 otherwise
 */
int get_process_info(unsigned int pid) {
	DIR * proc_dir = opendir("/proc/");
	struct dirent * current_dir;

	while((current_dir = readdir(proc_dir)) != NULL) {
		if(atoi(current_dir->d_name) == pid) {
			fill_module(current_dir);
			return 0;
		}
	}
	return -1;
}
