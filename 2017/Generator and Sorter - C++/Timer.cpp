#include "Timer.h"
#include <stdio.h>
#include <time.h>

#define BILLION 1000000000

using namespace std;

Timer::Timer() {
}

//Sets the start time

void Timer::setStart() {
	clock_gettime(CLOCK_MONOTONIC, &startTime);
}

//Sets the end time

void Timer::setEnd() {
	clock_gettime(CLOCK_MONOTONIC, &endTime);
}

//Calculates the elapsed time and prints it to the terminal.

void Timer::printTime() {
	timeElapsed = BILLION * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_nsec - startTime.tv_nsec);
	fprintf(stderr, "Time elapsed: %.7f seconds\n", (float)timeElapsed/BILLION);
}
