#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include "cmockery/cmockery.h"

typedef struct _childs {
	int fd[2];
	int pid;
	int OK;
} childs;

childs create_fork() {
	childs ret;
	if(pipe(ret.fd) < 0) {
		ret.OK = 0;
		return ret;
	}
	ret.OK = 1;
	ret.pid = fork();
	if(ret.pid == 0) {
		close(ret.fd[0]);
		dup2(ret.fd[1], STDOUT_FILENO);
		dup2(ret.fd[1], STDERR_FILENO);
		close(ret.fd[1]);
	} else {
		close(ret.fd[1]);
	}
	return ret;
}

void test_no_param() {
	childs ch = create_fork();
	assert(ch.OK);

	if(ch.pid == 0) {
		char * args[] = {"./gcov/schedule", NULL };
		execv(args[0], args);
		exit(1);
	}
	int ret;
	waitpid(ch.pid, &ret, 0);
	ret = WEXITSTATUS(ret);
	assert(ret != 0 && ret != 1);

	char str[255];
	read(ch.fd[0], str, 255);

	str[33] = '\0';
	assert_string_equal(str, "Invalid parameter count! usage is");
	
	close(ch.fd[0]);
}

void test_no_file() {
	childs ch = create_fork();
	assert(ch.OK);

	if(ch.pid == 0) {
		char * args[] = {"./gcov/schedule", "thisfiledoesntexist.txx", NULL};
		execv(args[0], args);
		exit(1);
	}
	int ret;
	waitpid(ch.pid, &ret, 0);
	ret = WEXITSTATUS(ret);
	assert(ret != 0 && ret != 1);

	char str[255];
	read(ch.fd[0], str, 255);
	str[14] = '\0';
	assert_string_equal(str, "File not found");
	
	close(ch.fd[0]);
}

void test_bad_file() {
	childs ch = create_fork();
	assert(ch.OK);

	if(ch.pid == 0) {
		char * args[] = {"./gcov/schedule", "makefile", NULL};
		execv(args[0], args);
		exit(1);
	}
	int ret;
	waitpid(ch.pid, &ret, 0);
	ret = WEXITSTATUS(ret);
	assert(ret != 0 && ret != 1);


	char str[255];
	read(ch.fd[0], str, 255);
	str[24] = '\0';
	assert_string_equal(str, "File format is incorrect");
	
	close(ch.fd[0]);
}

void test_good_file() {
	childs ch = create_fork();
	assert(ch.OK);

	if(ch.pid == 0) {
		char * args[] = {"./gcov/schedule", "test.txt", NULL};
		execv(args[0], args);
		exit(1);
	}
	int ret;
	kill(ch.pid, SIGINT);
	waitpid(ch.pid, &ret, 0);
	ret = WEXITSTATUS(ret);
	assert_int_equal(ret, 0);

	close(ch.fd[0]);
}

int main(int argc, char **argv) {
	const UnitTest tests[] = {
		unit_test(test_no_param),
		unit_test(test_no_file),
		unit_test(test_bad_file),
		unit_test(test_good_file),
	};
	return run_tests(tests, "run");
}

