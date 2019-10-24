#include <stdio.h>
#include <stdlib.h>
#include "../../include/global.h"
#include "../../include/fileparser.h"

#define USAGE "schedule <filename>"

struct {
	process_table * table;
} _module;

void init_module() {
	
}

int main(int argc, char **argv) {
	/* Make continuous error return */
	ERR_CODE err = OK;
	
	/* Check correct number of parameters */
	if(argc != 2) {
		fprintf(stderr, 
			"Invalid parameter count! usage is: " USAGE "\n");
		exit(-1);
	}

	/* Initialize module */
	init_module();
	
	/* Parse file and fill table */
	err = parse_file(argv[1], &_module.table);

	/* Print errors if there are errors */
	switch(err) {
		case OK: break; // Everything OK
		case INVALID_FILE:
			 fprintf(stderr, "File not found\n");
			 break;
		case INVALID_FILE_FORMAT:
			 fprintf(stderr, "File format is incorrect\n");
			 break;
	}
	
	/* We have to free memory allocated by parse_file */
	free(_module.table);
	return 0;
}
