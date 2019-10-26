#ifndef _FILEPARSER_H_
#define _FILEPARSER_H_

#include "global.h"

typedef struct _process_table {
	int process_number;
	int arrival_time;
	int burst;
	int priority;
} process_table;

ERR_CODE parse_file(char * filename, process_table ** table, int * entry_count);


#endif //_FILEPARSER_H_
