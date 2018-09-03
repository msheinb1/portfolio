#ifndef SORTER_H
#define SORTER_H
#include <stdio.h>

class Sorter
{

	public:
	int * numArr;
	int arrSize;

	Sorter(int * array, int numInts);
	void print();
	int * getArr();
	int getArrSize();
	int numInArr(int search);
	void cleanup();
};
#endif
