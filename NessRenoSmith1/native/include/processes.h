#ifndef _PROCESSES_H_
#define _PROCESSES_H_

int get_process_info(unsigned int pid);

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
	unsigned tty_h : 1;	//!< Append the TTY column
	unsigned time_h : 1;	//!< Append the TIME column
	unsigned cmd_h : 1;	//!< Append the CMD column
}process_info; //!< Struct that holds header information


#endif // _PROCESSES_H_
