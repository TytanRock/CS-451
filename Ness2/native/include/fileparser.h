/**
 * Author: Cory
 * Assignment Number: 2
 * Date of Submission: 11/6/2019
 * Name of this file: fileparser.h
 * Description of the program:
 *  This program runs a scheduler on a set of prime-finders
 *  in order to show how a priority scheduler works
 */
#ifndef _FILEPARSER_H_
#define _FILEPARSER_H_

#include "global.h"

/**
 * Struct that holds all the data from our process file 
 */
typedef struct _process_table {
	int process_number; //!< ID of process
	int arrival_time; //!< Time process arrived
	int burst;
	int priority;
} process_table;

/**
 * parse_file
 * Purpose: Parse the given filename into an array of process_table's
 * Parameters:
 *  filename - name of file to parse
 *  table - Pointer to array of process_tables
 *  entry_count - int pointer that gets filled with the total number of entries
 * Returns: Error code based on success of function
 */
ERR_CODE parse_file(char * filename, process_table ** table, int * entry_count);


#endif //_FILEPARSER_H_
