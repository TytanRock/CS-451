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
	int tty;		//!< TTY owner
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
	
	char file_path[50];
	strcpy(file_path, "");
	
	int clock_ticks_per_sec = sysconf(_SC_CLK_TCK);

	/* Open stat file to find general statistics */
	sprintf(file_path, "/proc/%d/stat", pid);
	
	FILE * stat_file = fopen(file_path, "r");
	if(stat_file == NULL) {
		/* Shouldn't happen, but if it does we're in trouble */
		return STATFILE_NOTPRESENT;
	}
	
	/* Find the PID, CMD, and State */
	fscanf(stat_file, "%d %s %c", &(_module.pid), _module.cmd, &(_module.state));
	/* Bypass ppid, pgrp, session */
	fscanf(stat_file, "%*d %*d %*d");
	fscanf(stat_file, "%d", &(_module.tty)); // Find the TTY
	/* Bypass tpgid, flags, minflt, cminflt, maiflt, cmaiflt */
	fscanf(stat_file, "%*d %*u %*d %*d %*d %*d"); 
	
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

	fscanf(cmdline_file, "%s", _module.cmdline);
	
	fclose(cmdline_file); // Always close your file

	return OK;
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
	DIR * proc_dir = opendir("/proc/");
	struct dirent * current_dir;

	memcpy(&(_module.header_info), headers, sizeof(process_info));

	while((current_dir = readdir(proc_dir)) != NULL) {
		if(atoi(current_dir->d_name) == pid) {
			return fill_module(pid);
		}
	}
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
	if(_module.header_info.state_h) {
		/* Append the state header */
		append_string(ret_string, offset, sz, "    S ");
	}
	if(1) {
		/* Append PID number */
		append_string(ret_string, offset, sz, "   PID ");
	}
	if(_module.header_info.mem_h) {
		/* Append the memory */
		append_string(ret_string, offset, sz, "       SZ ");
	}
	if(_module.header_info.time_h) {
		/* Append the time */
		append_string(ret_string, offset, sz, "     TIME ");
	}
	if(_module.header_info.cmd_h) {
		/* Append the command */
		append_string(ret_string, offset, sz, "CMD");
	}
	
	/* Ensure newline is added */
	append_string(ret_string, offset, sz, "\n");

	return OK;
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
	if(_module.header_info.state_h) {
		/* Fill with state info */
		append_string(ret_string, offset, sz, " %4c ", _module.state);
	}
	if(1) {
		/* Fill with pid number */
		append_string(ret_string, offset, sz, " %5d ", _module.pid);
	}

	if(_module.header_info.mem_h) {
		/* Fill with memory */
		append_string(ret_string, offset, sz, " %8d ", _module.mem);
	}

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

	if(_module.header_info.cmd_h) {
		append_string(ret_string, offset, sz, "%-s", _module.cmdline);
	}
	
	/* Ensure newline is at end of string */
	append_string(ret_string, offset, sz, "\n");
	
	return OK; // This will always return OK
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
	ERR_CODE ret_err = OK;
	
	int size = 1; /* Begin at an arbitrarily small value */
	int offset = 0; /* Begin at zero */
	*ret_string = calloc(sizeof(char), size); /* Malloc the size of the string */
	
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


