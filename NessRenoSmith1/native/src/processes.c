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
	char cmd[255];		//!< CMD that called it
}_module; //!< Module-specific information


int fill_module(unsigned int pid) {
	memset(_module.cmd, 0, 255);
	
	char stat_path[50];
	strcpy(stat_path, "");
	
	/* Build the path to the stat file */
	sprintf(stat_path, "/proc/%d/stat", pid);
	
	/* Open the stat file */
	FILE * stat_file = fopen(stat_path, "r");
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
	
	printf("%s\n", _module.cmd);
	
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
 *  0 for success, -1 otherwise
 */
int get_process_info(unsigned int pid) {
	DIR * proc_dir = opendir("/proc/");
	struct dirent * current_dir;

	while((current_dir = readdir(proc_dir)) != NULL) {
		if(atoi(current_dir->d_name) == pid) {
			fill_module(pid);
			return 0;
		}
	}
	return -1;
}
