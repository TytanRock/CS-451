#include "../include/printing.h"

#include <stdio.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <semaphore.h>

static struct {
	int current_time;
	sem_t stdout_semaphore;
} _module;

void timer_handler(int signal) {
	_module.current_time ++;
}

void initialize_printing() {
	_module.current_time = 0;
	signal(SIGALRM, timer_handler);
	struct itimerval timer;
	timer.it_value.tv_sec = 1;
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = 1;
	timer.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &timer, NULL);

	sem_init(&_module.stdout_semaphore, 0, 1);
}

void timed_print(const char *format, ...) {
	sem_wait(&_module.stdout_semaphore);
	va_list args;
	va_start(args, format);

	printf("%2d: ", _module.current_time);
	vprintf(format, args);
	
	va_end(args);
	sem_post(&_module.stdout_semaphore);
}
