# Assignment Number 1

Authors: Cory Ness

Project: Write your own PS program

Guidelines: Write a PS (Process Identifier) program that accepts up to 5 command line arguments. Those arguments are:
 - p <pid>: Display process information for the process number is 'pid'. If this option is not present, use a pid number of 1. If user runs -p with no pid number, give an error message and exit the program.
 - s: Display the single-character state information about the process. This information is found in the `stat`Project: Write your own PS program

Guidelines: Write a PS (Process Identifier) program that accepts up to 5 command line arguments. Those arguments are:
 - p <pid>: Display process information for the process number is `pid`. If this option is not present, use a pid number of 1. If user runs -p with no pid number, give an error message and exit the program.
 - s: Display the single-character state information about the process. This information is found in the `stat` file in the process's directory, looking at the third ("state") field. Not the information read is a character string. If this option is not present do not display the information.
 - t: Display the amount of time consumed by this process in `hours:min:seconds` format. This information can be found in the `stat` file in process's directory, looking at the "utime" field. Add to this the system time, "stime" field. This is the total time consumed by the process, in clock ticks.If this option is not present, do not display the information.
 - v: Display the amount of virtual memory currently being used (in pages) by this program. This information can be found in the `statm` file in process's directory, looking at first ("size") field. If this option is not present, do not display the information.
 - c: Display the command line tha tstarted this program. This information is found in the cmdline file in process's directory. Be careful on this one, because this file contains a list of null terminated strings. If this option is not present, do not display this information.

**All of your output must match with the output of "ps -ely" for the same pid.**

