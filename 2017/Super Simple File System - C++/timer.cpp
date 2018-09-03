#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "timer.h"
#include <errno.h>


double current_time(){
	double retval;
	int sec;
	double micro;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	sec = tv.tv_sec;
	micro = tv.tv_usec;
	retval = sec + (micro/1000000.0);
	return retval;
}

double time_difference(double begin, double end){
	return end-begin;
}
