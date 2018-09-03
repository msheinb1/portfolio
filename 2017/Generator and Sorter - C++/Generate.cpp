#include "Generate.h"
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>

using namespace std;

/*
Constructor.
Takes a minimum integer value, a maximum integer value, and a third integer that will become the size of the array as arguments.
It takes the computer time and saves it as a seed, then seeds the random number generator.
It attempts to dynamically create an array with a size matching the numInts field.
If the third value is less than or equal to 0, throws an error and exits the program.
*/

Generate::Generate(int min, int max, int numInts) {
	this->min = min;
	this->max = max;
	this->numInts = numInts;

	if (numInts <= 0) {
		fprintf(stderr, "You tried to make an array smaller than 1.\n");
		exit(1);
	}

	timeval t;
	gettimeofday(&t, NULL);
	seed = t.tv_usec * t.tv_sec;

	numArr = (int *) malloc(numInts*sizeof(int));
	if (numArr == NULL) {
		fprintf(stderr, "Error with generating array.\n");
		exit(1);
	}
	seedRNG();
}

/*
Constructor.
Takes a minimum integer value, a maximum integer value, and a third integer that will become the size of the array, takes a unsigned long integer as a seed as arguments.
It attempts to dynamically create an array with a size matching the numInts field.
It seeds the random number generator using the seed provided by the user.
If the third value is less than or equal to 0, throws an error and exits the program.
*/

Generate::Generate(int min, int max, int numInts, unsigned long seed) {
	this->min = min;
	this->max = max;
	this->numInts = numInts;

	if (numInts <= 0) {
		fprintf(stderr, "You tried to make an array smaller than 1.\n");
		exit(1);
	}

	this->seed = seed;
	
	numArr = (int *) malloc(numInts*sizeof(int));
	if (numArr == NULL) {
		fprintf(stderr, "Error with generating array.\n");
		exit(1);
	}
	seedRNG();
}

//Seeds the pseudo-random number generator

void Generate::seedRNG() {
	srand(seed);
}

//Fills the array with random numbers between the maximum and minimum numbers provided when the generator was created.

void Generate::populate() {
	for (int i = 0; i < numInts; i++) {
		numArr[i] = (rand() % max) + min;
	}
}


//Prints the values in the array to the terminal


void Generate::printVals() {
	int check = 0;
		check = printf("%d\n", numInts);
		if (printf < 0) {
			fprintf(stderr, "Printing values failed.\n");
			exit(1);
		}
	for (int i = 0; i < numInts; i++) {
		check = printf("%d\n", numArr[i]);
		if (printf < 0) {
			fprintf(stderr, "Printing values failed.\n");
			exit(1);
		}
	}
}

/*
Takes a file stream argument, and attempts to open it.
Prints the values in the array to the given file stream.
If the file stream cannot be opened the program closes.
*/

void Generate::printVals(char *f) {
	FILE * outfile = fopen(f, "w");
	if (outfile == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		exit(1);
	}
	int check = 0;
		check = fprintf(outfile, "%d\n", numInts);
		if (check < 0) {
			fprintf(stderr, "Printing values failed.\n");
			exit(1);
		}
	for (int i = 0; i < numInts; i++) {
		check = fprintf(outfile, "%d\n", numArr[i]);
		if (check < 0) {
			fprintf(stderr, "Printing values failed.\n");
			exit(1);
		}
	}
	fclose(outfile);
}

//Frees the dynamically allocated memory associated with the array

void Generate::cleanup() {
	free(numArr);
}
