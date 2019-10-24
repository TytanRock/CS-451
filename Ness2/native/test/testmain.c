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

void test_no_file() {
	childs ch = create_fork();
	assert(ch.OK);

	if(ch.pid == 0) {
		char * args[] = {"./gcov/schedule", NULL };
		execv(args[0], args);
		exit(1);
	}
	int ret;
	waitpid(ch.pid, &ret, 0);
	assert_int_not_equal(ret, 0);
	assert_int_not_equal(ret, 1);

	char str[255];
	read(ch.fd[0], str, 255);

	assert_string_equal(str, "Invalid parameter count! usage is: schedule <filename>\n");
	
	close(ch.fd[0]);
}

int main(int argc, char **argv) {
	const UnitTest tests[] = {
		unit_test(test_no_file),
	};
	return run_tests(tests, "run");
}

