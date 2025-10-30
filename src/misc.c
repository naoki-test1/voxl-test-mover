#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include "misc.h"


int64_t my_time_monotonic_ns()
{
	struct timespec ts;
	if(clock_gettime(CLOCK_MONOTONIC, &ts)){
		fprintf(stderr,"ERROR calling clock_gettime\n");
		return -1;
	}
	return (int64_t)ts.tv_sec*1000000000 + (int64_t)ts.tv_nsec;
}

int64_t my_time_realtime_ns()
{
	struct timespec ts;
	if(clock_gettime(CLOCK_REALTIME, &ts)){
		fprintf(stderr,"ERROR calling clock_gettime\n");
		return -1;
	}
	return (int64_t)ts.tv_sec*1000000000 + (int64_t)ts.tv_nsec;
}


// sleep and timing functions, TODO these should go into a misc.c!
void my_nanosleep(uint64_t ns){
	struct timespec req,rem;
	req.tv_sec = ns/1000000000;
	req.tv_nsec = ns%1000000000;
	// loop untill nanosleep sets an error or finishes successfully
	errno=0; // reset errno to avoid false detection
	while(nanosleep(&req, &rem) && errno==EINTR){
		req.tv_sec = rem.tv_sec;
		req.tv_nsec = rem.tv_nsec;
	}
	return;
}


int my_loop_sleep(double rate_hz, int64_t* next_time)
{
	int64_t current_time = my_time_realtime_ns();

	// static variable so we remember when we last woke up
	if(*next_time<=0){
		*next_time = current_time;
	}

	// try to maintain output data rate
	*next_time += (1000000000.0/rate_hz);

	// uh oh, we fell behind, reset
	if(*next_time<=current_time){
		*next_time = current_time + (1000000000.0/rate_hz);
		return 0;
	}

	// normal operation
	my_nanosleep(*next_time-current_time);
	return 0;
}

