# Assignment Number 1

**Authors**: Cory Ness

**Project**: Write your own PS program

## How to Build

 1. Type `make` in main directory
 2. Makefile will do its thing, and place the binary into the `bin/` folder
 3. `cd` into `bin/`
 4. Type `./5ps [arguments] [-p <pid number>]` to test the program

## Project (Used for developer only)

**Guidelines**: Write a PS (Process Identifier) program that accepts up to 5 command line arguments. Those arguments are:
 - p <pid>: Display process information for the process number is `pid`. If this option is not present, use a pid number of 1. If user runs -p with no pid number, give an error message and exit the program.
 - s: Display the single-character state information about the process. This information is found in the `stat` file in the process's directory, looking at the third ("state") field. Not the information read is a character string. If this option is not present do not display the information.
 - t: Display the amount of time consumed by this process in `hours:min:seconds` format. This information can be found in the `stat` file in process's directory, looking at the "utime" field. Add to this the system time, "stime" field. This is the total time consumed by the process, in clock ticks.If this option is not present, do not display the information.
 - v: Display the amount of virtual memory currently being used (in pages) by this program. This information can be found in the `statm` file in process's directory, looking at first ("size") field. If this option is not present, do not display the information.
 - c: Display the command line tha tstarted this program. This information is found in the cmdline file in process's directory. Be careful on this one, because this file contains a list of null terminated strings. If this option is not present, do not display this information.

**All of your output must match with the output of "ps -ely" for the same pid.**

## Desired structure
The desired structure for this project is broken down as follows:
 - A main.c that parses user input, passes parsed input into modules, and prints characters back to user
 - A processes.c that parses the stat and statm files and passes back to caller

## Testing
This project will use the following tests to ensure it is operating correctly.
 - Ensure 5ps successfully finds pid 1
 - Ensure 5ps does not crash under any circumstances
 - Ensure 5ps returns 0 with all valid arguments
 - Ensure 5ps matches the output of `ps -ely` for any given PID

# Resources
This README used the documentation provided in the assignment for some of the sections.

The unit-testing code was also provided by cmocka at https://cmocka.org/, licensed under the Apache 2.0 license. 

