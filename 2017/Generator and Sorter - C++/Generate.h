#ifndef GENERATE_H
#define GENERATE_H
#include <stdio.h>

class Generate
{
	public:
	int min;
	int max;
	int numInts;
	unsigned long seed;

	int * numArr;
	
	Generate(int min, int max, int numInts);
	Generate(int min, int max, int numInts, unsigned long seed);
	void seedRNG();
	void populate();
	void printVals();
	void printVals(char *f);
	void cleanup();

};
#endif
