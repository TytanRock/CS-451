#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include "cmockery/cmockery.h"
#include "../include/processes.h"

/* Extern'd function to test and ensure works */
extern void append_string(char ** str, int * offset, int * sz, char * format, ...);

void test_processes_headers() {
	process_info tmpInfo;
	/* Always returns 0 because ID 1 is always valid */
	assert_int_equal(get_process_info(1, &tmpInfo), 0);
}

void test_run_process() {
	int fg[2];
	if(pipe(fg) < 0) {
		fail();
	}
	int pid = fork();
	if(pid == 0) {
		dup2(fg[1], STDOUT_FILENO);
		close(fg[1]);
		close(fg[0]);
		char *args[] = {"./gcov/5ps", "-p", "1", NULL};
		execv("./gcov/5ps", args);
		exit(1);
	}
	close(fg[1]);
	int ret;
	waitpid(pid, &ret, 0);
	/* Call the actual gcovary */
	assert_int_equal(ret, 0);
	close(fg[0]);
}

void test_append_works() {
	/* Initialize string */
	char ** str;
	char * tmp;
	str = &tmp;

	int sz = 1;
	int offset = 0;
	*str = malloc(sz);
	
	append_string(str, &offset, &sz, "This is a test string %s!\n%d is %010d\n", "to test the function", 10, 10);

	assert_int_not_equal(sz, 1);
	assert_int_not_equal(offset, 0);
	assert_string_equal(*str, "This is a test string to test the function!\n10 is 0000000010\n");
}

void test_invalid_pid() {
	int fg[2];
	if(pipe(fg) < 0) {
		fail();
	}
	
	int pid = fork();
	if(pid == 0) {
		close(fg[0]);
		dup2(fg[1], STDERR_FILENO);
		close(fg[1]);
		char *args[] = {"./gcov/5ps", "-p", "-1", NULL};
		execv("./gcov/5ps", args);
		exit(1);
	}
	close(fg[1]);
	int ret;
	waitpid(pid, &ret, 0);
	assert_int_not_equal(ret, 0);
	assert_int_not_equal(ret, 1);

	char str[255];
	read(fg[0], str, 255);
	
	str[15] = 0;

	assert_string_equal(str, "PID is invalid\n");
	close(fg[0]);
}

void test_all_parameters() {
	int fg[2];
	if(pipe(fg) < 0) {
		fail();
	}
	int pid = fork();
	if(pid == 0) {
		dup2(fg[1], STDOUT_FILENO);
		close(fg[1]);
		close(fg[0]);
		char *args[] = {"./gcov/5ps", "-stvcp", "1", NULL};
		execv("./gcov/5ps", args);
		exit(1);
	}
	close(fg[1]);
	int ret;
	waitpid(pid, &ret, 0);
	assert_int_equal(ret, 0);
	close(fg[0]);
}

void test_help() {
	int fg[2];
	if(pipe(fg) < 0) {
		fail();
	}
	
	int pid = fork();
	if(pid == 0) {
		close(fg[0]);
		dup2(fg[1], STDERR_FILENO);
		dup2(fg[1], STDOUT_FILENO);
		close(fg[1]);
		char *args[] = {"./gcov/5ps", "-h", NULL};
		execv("./gcov/5ps", args);
		exit(1);
	}
	close(fg[1]);
	int ret;
	waitpid(pid, &ret, 0);
	assert_int_equal(ret, 0);

	close(fg[0]);
}

void test_invalid_param() {
	int fg[2];
	if(pipe(fg) < 0) {
		fail();
	}
	
	int pid = fork();
	if(pid == 0) {
		close(fg[0]);
		dup2(fg[1], STDERR_FILENO);
		dup2(fg[1], STDOUT_FILENO);
		close(fg[1]);
		char *args[] = {"./gcov/5ps", "-zp", "d", NULL};
		execv("./gcov/5ps", args);
		exit(1);
	}
	close(fg[1]);
	int ret;
	waitpid(pid, &ret, 0);
	assert_int_not_equal(ret, 0);
	assert_int_not_equal(ret, 1);

	close(fg[0]);
}

void test_no_params() {
	int fg[2];
	if(pipe(fg) < 0) {
		fail();
	}
	
	int pid = fork();
	if(pid == 0) {
		close(fg[0]);
		dup2(fg[1], STDERR_FILENO);
		dup2(fg[1], STDOUT_FILENO);
		close(fg[1]);
		char *args[] = {"./gcov/5ps", NULL};
		execv("./gcov/5ps", args);
		exit(1);
	}
	close(fg[1]);
	int ret;
	waitpid(pid, &ret, 0);
	assert_int_equal(ret, 0);

	close(fg[0]);
}

int main() {
	const UnitTest tests[] = {
		unit_test(test_processes_headers),
		unit_test(test_run_process),
		unit_test(test_append_works),
		unit_test(test_invalid_pid),
		unit_test(test_all_parameters),
		unit_test(test_help),
		unit_test(test_invalid_param),
		unit_test(test_no_params),
	};
	return run_tests(tests, "run");
}

