#ifndef _PROCESSES_H_
#define _PROCESSES_H_

int get_process_info(unsigned int pid);

typedef struct _process_header {
	unsigned pid_h : 1;	//!< Append the PID column
	unsigned tty_h : 1;	//!< Append the TTY column
	unsigned time_h : 1;	//!< Append the TIME column
	unsigned cmd_h : 1;	//!< Append the CMD column
}process_info; //!< Struct that holds header information


#endif // _PROCESSES_H_
