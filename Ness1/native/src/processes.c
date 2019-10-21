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

#include "../include/processes.h"
#include "../include/stringwork.h"
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_STR_SIZE 255

struct {
	struct _process_header header_info;
	
	int pid;		//!< PID Number
	char state;		//!< State of process
	unsigned long time;	//!< Time alive
	char cmd[MAX_STR_SIZE];	//!< Name of command

	int mem;		//!< Amount of virtual memory process takes
	
	char cmdline[MAX_STR_SIZE];//!< CMD that called it
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
 * Returns: ERR_CODE based on status
 */
ERR_CODE fill_module(unsigned int pid) {
	/* Initialize module */
	memset(_module.cmd, 0, 255);
	
	/* Initialize function variables */
	char file_path[50];
	strcpy(file_path, "");
	int clock_ticks_per_sec = sysconf(_SC_CLK_TCK);

	/* Open stat file to find general statistics */
	sprintf(file_path, "/proc/%d/stat", pid);
	
	/* Open the stat file */
	FILE * stat_file = fopen(file_path, "r");
	if(stat_file == NULL) {
		/* Shouldn't happen, but if it does we're in trouble */
		return STATFILE_NOTPRESENT;
	}
	
	/* Find the PID, CMD, and State */
	/* Bypass ppid, pgrp, session, TTY */
	/* Bypass tpgid, flags, minflt, cminflt, maiflt, cmaiflt */
	fscanf(stat_file, "%d %s %c", &(_module.pid), _module.cmd, &(_module.state));
	fscanf(stat_file, "%*d %*d %*d %*d");
	fscanf(stat_file, "%*d %*u %*d %*d %*d %*d"); 
	
	/* Get time process is running */
	unsigned long utime;
	fscanf(stat_file, "%lu", &utime); // Find utime
	utime /= clock_ticks_per_sec; // Make sure it's in seconds
	unsigned long stime;
	fscanf(stat_file, "%lu", &stime); // Find stime
	stime /= clock_ticks_per_sec; // Make sure it's in seconds
	/* Add utime and stime together */
	_module.time = utime + stime;

	fclose(stat_file); //!< Always close the file when we're done

	/* Now open statm file to find memory statistics */
	sprintf(file_path, "/proc/%d/statm", pid);
	FILE * statm_file = fopen(file_path, "r");
	if(statm_file == NULL) {
		/* Shouldn't happen, but if it does we're in trouble */
		return STATMFILE_NOTPRESENT;
	}
	
	fscanf(statm_file, "%d", &(_module.mem)); // Find the mem (in pages) of process

	fclose(statm_file); // Always close your file
	
	/* Now open cmdline file to find cmdline information */
	sprintf(file_path, "/proc/%d/cmdline", pid);
	FILE * cmdline_file = fopen(file_path ,"r");
	if(cmdline_file == NULL) {
		/* Shouldn't happen, but if it does we're in trouble */
		return CMDLINEFILE_NOTPRESENT;
	}

	fscanf(cmdline_file, "%s", _module.cmdline); // Find cmdline
	
	fclose(cmdline_file); // Always close your file

	/* Fix CMD to remove the parenthesis */
	strcpy(_module.cmd, _module.cmd + 1); // Remove first char
	_module.cmd[strlen(_module.cmd) - 1] = '\0'; // Remove last char

	return OK; // We made it through, return OK
}

/**
 * get_process_info
 * Purpose:
 *  Checks if process exists and calls the fill function if it does
 *
 * Parameters:
 *  pid - ID number of process to get information from
 *
 * Returns: ERR_CODE based on status of function
 */
ERR_CODE get_process_info(unsigned int pid, process_info *headers) {
	/* Check if pid is 0, if it is we know it's bad */
	/* This is done here instead in the checking because atoi will return 0 for a bad parse */
	if(pid == 0) {
		return PID_INVALID;
	}
	
	/* Open directory and initialize dirent struct */
	DIR * proc_dir = opendir("/proc/");
	struct dirent * current_dir;

	/* Ensure our module variables are cleared */
	memcpy(&(_module.header_info), headers, sizeof(process_info));

	/* Check every directory inside /proc to see if it matches the pid number */
	while((current_dir = readdir(proc_dir)) != NULL) {
		/* If the directory matches our pid, use it in fill_module */
		if(atoi(current_dir->d_name) == pid) {
			return fill_module(pid);
		}
	}

	/* We didn't find the directory, tell user their pid is invalid */
	return PID_INVALID;
}

/**
 * generate_headers
 * Purpose:
 *  Generate column headers based on the user-specified flags
 * Parameters:
 *  ret_string - pointer to string to fill
 *  offset - pointer to current string offset
 *  sz - pointer to current string size
 * Returns:
 *  ERR_CODE based on state of function
 */
ERR_CODE generate_headers(char ** ret_string, int * offset, int * sz) {
	/* Check if we need to append state */
	if(_module.header_info.state_h) {
		/* Append the state header */
		append_string(ret_string, offset, sz, "    S ");
	}
	/* Check if we need to append PID number */
	if(_module.header_info.pid_h) {
		/* Append PID number */
		append_string(ret_string, offset, sz, "   PID ");
	}
	/* Check if we need to append memory */
	if(_module.header_info.mem_h) {
		/* Append the memory */
		append_string(ret_string, offset, sz, "       SZ ");
	}
	/* Check if we need to append time */
	if(_module.header_info.time_h) {
		/* Append the time */
		append_string(ret_string, offset, sz, "     TIME ");
	}
	/* Check if we need to append cmd */
	if(_module.header_info.cmd_h) {
		/* Append the command */
		append_string(ret_string, offset, sz, "CMD");
	}
	
	/* Ensure newline is added */
	append_string(ret_string, offset, sz, "\n");

	return OK; // We made it through, return OK
}

/**
 * fill_stats
 * Purpose:
 *  Fill the string based on the module's parsed stats
 * Parameters:
 *  ret_string - pointer to string to fill
 *  offset - pointer to the current offset of the string
 *  sz - pointer to the current size of the string
 * Returns:
 *  ERR_CODE based on success or fail
 */
ERR_CODE fill_stats(char ** ret_string, int * offset, int * sz) {
	/* Check if we need to append state */
	if(_module.header_info.state_h) {
		/* Fill with state info */
		append_string(ret_string, offset, sz, " %4c ", _module.state);
	}
	/* Check if we need to append PID number */
	if(_module.header_info.pid_h) {
		/* Fill with pid number */
		append_string(ret_string, offset, sz, " %5d ", _module.pid);
	}
	/* Check if we need to append memory */
	if(_module.header_info.mem_h) {
		/* Fill with memory */
		append_string(ret_string, offset, sz, " %8d ", _module.mem);
	}
	/* Check if we need to append time */
	if(_module.header_info.time_h) {
		/* Fill with time spent */
		int hours = _module.time / 3600;
		int mins = (_module.time /60) % 360;
		int secs = _module.time % 60;
		append_string(ret_string, offset, sz, " %02d:%02d:%02d ", 
				hours, 
				mins, 
				secs);
	}
	/* Check if we need to append cmd */
	if(_module.header_info.cmd_h) {
		append_string(ret_string, offset, sz, "%-s", _module.cmd);
	}
	
	/* Ensure newline is at end of string */
	append_string(ret_string, offset, sz, "\n");
	
	return OK; // We made it to the end, return OK
}

/**
 * produce_pid_info
 * Purpose:
 *  Generate the pid information based on the header flags 
 *  mand the pid information
 *
 * Parameters:
 *  ret_string - Pointer to char pointer, will be malloc'd and needs to be free'd later
 *
 * Returns: ERR_CODE based on result of function
 */
ERR_CODE produce_pid_info(char ** ret_string) {
	/* Initialize function variables */
	ERR_CODE ret_err = OK; // Initialize return error variable
	int size = 1; /* Begin at an arbitrarily small value */
	int offset = 0; /* Begin at zero */
	*ret_string = calloc(sizeof(char), size); /* calloc the size of the string */
	
	/* If we haven't err'd yet, generate headers */
	if(ret_err == OK) {
		ret_err = generate_headers(ret_string, &offset, &size);
	}
	
	/* If we haven't err'd yet, fill stats */
	if(ret_err == OK) {
		ret_err = fill_stats(ret_string, &offset, &size);
	}
	
	/* Let caller know if we err'd */
	return ret_err;
}


