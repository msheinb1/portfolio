#ifndef TIMER_H
#define TIMER_H
#include <time.h>

class Timer
{
	public:
	struct timespec startTime;
	struct timespec endTime;
	unsigned long long timeElapsed;

	Timer();
	void setStart();
	void setEnd();
	void printTime();
};
#endif
