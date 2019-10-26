#include <stdlib.h>
#include <stdio.h>
#include "../../include/fileparser.h"

ERR_CODE parse_file(char * filename, process_table ** table, int *entry_count) {
	/* Read file and begin parsing */
	FILE * file = fopen(filename, "r");
	/* initialize entry_count */
	*entry_count = 0;
	
	/* Check file is valid */
	if(file == NULL) {
		return INVALID_FILE;
	}

	/* Find how many entries are needed in the table */
	unsigned int line_count;
	{
		char ch;
		while((ch = fgetc(file)) != EOF) {
			if(ch == '\n') ++line_count;
		}
	}
	fseek(file, 0, SEEK_SET); // Re-set file to beginning

	/* Allocate space for process table */
	*table = malloc(line_count * sizeof(process_table));

	/* Parse every line and fill the process table */
	for(int i = 0; i < line_count; ++i) {
		int proc_num;
		int arr_time;
		int brst;
		int priority;
		int matches = fscanf(file, "%d %d %d %d",  
				&proc_num, 
				&arr_time, 
				&brst, 
				&priority);
		/* If we don't have 4 matches, the file is incorrect */
		if(matches != 4 && matches != -1) {
			return INVALID_FILE_FORMAT;
		}
		if(matches == 4) {
			++(*entry_count);
			(*table)[i].process_number = proc_num;
			(*table)[i].arrival_time = arr_time;
			(*table)[i].burst = brst;
			(*table)[i].priority = priority;
		}
	}
	return OK;
}

