#ifndef _PROCESSES_H_
#define _PROCESSES_H_

typedef enum _ERR_CODE {
	OK = 0,
	PID_INVALID = -1,
	STATFILE_NOTPRESENT = -2,
	STATMFILE_NOTPRESENT = -3,
	CMDLINEFILE_NOTPRESENT = -4,

	OTHER_ERR = -10,
}ERR_CODE;

typedef struct _process_header {
	unsigned pid_h : 1;	//!< Append the PID column
	unsigned state_h : 1;	//!< Append the STATE column
	unsigned time_h : 1;	//!< Append the TIME column
	unsigned cmd_h : 1;	//!< Append the CMD column
	unsigned mem_h : 1;	//!< Apend the MEM column
}process_info; //!< Struct that holds header information

int get_process_info(unsigned int pid, struct _process_header * headerFlags);

#endif // _PROCESSES_H_
