/**
 * Author: Cory
 * Assignment Number: 1
 * Date of Submission: 10/23/2019
 * Name of this file: main.c
 * Description of the program:
 *  This program mimics the output of ps -ely for any specific pid
 *  It uses the /proc directory to find the process information that
 *  the user wants, specified by the 5 CLI parameters, p,s,t,v,c
 */


#ifndef _PROCESSES_H_
#define _PROCESSES_H_

/**
 * Set of enums that dictate possible fail conditions of
 * the functions used
 */
typedef enum _ERR_CODE {
	OK = 0,
	PID_INVALID = -1,
	STATFILE_NOTPRESENT = -2,
	STATMFILE_NOTPRESENT = -3,
	CMDLINEFILE_NOTPRESENT = -4,

	OTHER_ERR = -10,
}ERR_CODE;

/**
 * Shared typedef struct to pass user arguments between
 * main module and processes module
 */
typedef struct _process_header {
	unsigned pid_h : 1;	//!< Append the PID column
	unsigned state_h : 1;	//!< Append the STATE column
	unsigned time_h : 1;	//!< Append the TIME column
	unsigned cmd_h : 1;	//!< Append the CMD column
	unsigned mem_h : 1;	//!< Apend the MEM column
}process_info; //!< Struct that holds header information

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
ERR_CODE get_process_info(unsigned int pid, struct _process_header * headerFlags);

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
ERR_CODE produce_pid_info(char ** ret_string);

#endif // _PROCESSES_H_
