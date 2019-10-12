#include "../include/processes.h"
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include <stdio.h> // !< Remove after debugging is complete

struct {
	struct _process_header header_info;
	
	int pid;		//!< PID Number
	char state;		//!< State of process
	int tty;		//!< TTY owner
	unsigned long time;	//!< Time alive
	char cmd[255];		//!< Name of command

	int mem;		//!< Amount of virtual memory process takes
	
	char cmdline; 		//!< CMD that called it
}_module; //!< Module-specific information

/**
 * fill_module
 * Purpose:
 *  Local function that fills module variables with relevant information for
 *  the PS program
 *
 * Parameters:
 *  pid - PID number of process to check
 *
 * Returns:
 *  -1 if stat file for process fails to open
 *  -2 if statm file for process fails to open
 *  0 otherwise
 */
int fill_module(unsigned int pid) {
	/* Initialize module */
	memset(_module.cmd, 0, 255);
	
	char file_path[50];
	strcpy(file_path, "");
	
	/* Open stat file to find general statistics */
	sprintf(file_path, "/proc/%d/stat", pid);
	
	FILE * stat_file = fopen(file_path, "r");
	if(stat_file == NULL) {
		/* Shouldn't happen, but if it does we're in trouble */
		return -1;
	}
	
	int useless_int;

	fscanf(stat_file, "%d", &(_module.pid)); // Find the PID
		
	fscanf(stat_file, "%s", _module.cmd); // Find caller's name

	fscanf(stat_file, "%c", &(_module.state)); // Find the state

	fscanf(stat_file, "%d", &useless_int); // Bypass ppid

	fscanf(stat_file, "%d", &useless_int); // Bypass pgrp

	fscanf(stat_file, "%d", &useless_int); // Bypass session

	fscanf(stat_file, "%d", &(_module.tty)); // Find the TTY
	
	fclose(stat_file); //!< Always close the file when we're done

	/* Now open statm file to find memory statistics */
	sprintf(file_path, "/proc/%d/statm", pid);
	
	FILE * statm_file = fopen(file_path, "r");

	if(statm_file == NULL) {
		/* Shouldn't happen, but if it does we're in trouble */
		return -2;
	}
	
	fscanf(statm_file, "%d", &(_module.mem)); // Find the mem (in pages) of process

	fclose (statm_file);
	
	/* Now open cmdline file to find cmdline information */


	return 0;
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
 *  -10 for invalid pid number
 *  Result of fill_module otherwise
 */
int get_process_info(unsigned int pid) {
	DIR * proc_dir = opendir("/proc/");
	struct dirent * current_dir;

	while((current_dir = readdir(proc_dir)) != NULL) {
		if(atoi(current_dir->d_name) == pid) {
			return fill_module(pid);
		}
	}
	return -10;
}

