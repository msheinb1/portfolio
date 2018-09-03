#include "Sorter.h"
#include <stdlib.h>

/*
Constructor.
Takes an integer array, and an integer representing the size of the integer array being passed in as arguments.
Attempts to dynamically allocate memory to create an array.
Copies the contents of the integer array being passed in to the newly created array.
*/

Sorter::Sorter(int * array, int numInts) {
	numArr = (int *) malloc(numInts*sizeof(int));
	if (numArr == NULL) {
		fprintf(stderr, "Error with generating array.\n");
		exit(1);
	}
	arrSize = numInts;

	for (int i = 0; i < arrSize; i++) {
		numArr[i] = array[i];
	}
}

//Prints the values of the sorter's array to the terminal

void Sorter::print() {
	for (int i = 0; i < arrSize; i++) {
		printf("%d ", numArr[i]);
	}
	printf("\n");
}

//Returns the sorter's array

int * Sorter::getArr() {
	return numArr;
}

//Returns the size of the sorter's array

int Sorter::getArrSize() {
	return arrSize;
}

//Frees the sorter's array

void Sorter::cleanup() {
	free(numArr);
}

/*
Takes an integer value as an argument, and counts the number of times that integer appears in the sorted array.
Assumes the array is sorted.
*/

int Sorter::numInArr(int search) {
	int count  = 0;
	for (int i = 0; i < arrSize; i++) {
		if (numArr[i] > search) {
			break;
		} else if (numArr[i] == search) {
			count++;
		}
	}
	return count;
}
