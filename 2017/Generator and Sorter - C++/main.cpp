#include "Generate.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

using namespace std;

/*
Uses switch statements and getopt method to generate up to 1,000,000 random numbers from 0 to 1,000,000 and write them to the terminal or to an output file.
*/

int main(int argc, char *argv[]) {
	int numInts = 100;
	int minInt = 1;
	int maxInt = 255;
	unsigned long seed = -1;
	char * output = NULL;
	int command;
	int check = -1;

	int temp = 0;
	char *endptr;

	while ((command = getopt(argc, argv, "un:m:M:s:o:")) != -1) {
		switch (command) {
			case 'u':
				fprintf(stderr, "Usage: prog1generator -u -n <num-integers> -m <min int> -M <max-int> -s <seed> -o <output-filename>\n");
				exit(0);
			case 'n':
				temp = strtol(optarg, &endptr, 10);
				if ((errno == ERANGE && (temp == LONG_MAX || temp == LONG_MIN)) || (errno != 0 && temp == 0)) {
					fprintf(stderr, "strtol failed");
					exit(1);
				}

				if (temp < 0 ||temp > 1000000){
					fprintf(stderr, "You entered an invalid argument for flag -n. Available arguments are integers from 0 to 1,000,000\n");
					exit(1);
				}
				numInts = temp;
				break;
			case 'm':
				temp = strtol(optarg, &endptr, 10);
				if ((errno == ERANGE && (temp == LONG_MAX || temp == LONG_MIN)) || (errno != 0 && temp == 0)) {
					fprintf(stderr, "strtol failed");
					exit(1);
				}

				if (temp < 1) {
					fprintf(stderr, "You entered an invalid argument for flag -m. Available arguments are integers greater than 0\n");
					exit(1);
				}
				minInt = temp;
				break;
			case 'M':
				temp = strtol(optarg, &endptr, 10);
				if ((errno == ERANGE && (temp == LONG_MAX || temp == LONG_MIN)) || (errno != 0 && temp == 0)) {
					fprintf(stderr, "strtol failed");
					exit(1);
				}

				if (temp > 1000000) {
					fprintf(stderr, "You entered an invalid argument for flag -M. Available arguments are integers less than 1,000,000\n");
					exit(1);
				} else if (minInt != 1 && temp < minInt) {
					fprintf(stderr, "You entered an invalid argument for flag -M. You made max-int less than min-int.\n");
					exit(1);
				}
				maxInt = temp;
				break;
			case 's':
				unsigned int tempseed;
				tempseed = strtol(optarg, &endptr, 32);
				seed = tempseed;
				break;
			case 'o':
				output = optarg;
				break;
			case '?':
				if (((*optarg) == 'n') || ((*optarg) == 'm') || ((*optarg) == 'M') || ((*optarg) = 's') || ((*optarg) == 'o')) {
				check = fprintf(stderr, "Option -%c requires an argument. If you want the defaults, leave it out.\n", optopt);
					if (check < 0){
						fprintf(stderr, "Printing error message failed.\n");
						exit(1);
					}
				} else {
					check = fprintf(stderr, "Unknown option character '\\x%x'\n");
					if (check < 0){
						fprintf(stderr, "Printing value failed.\n");
					exit(1);
					}
				}
		}
	}

	Generate * myNums;
	if (seed != -1) {
		myNums = new Generate(minInt, maxInt, numInts, seed);
	} else {
		myNums = new Generate(minInt, maxInt, numInts);
	}

	myNums->populate();
	if (output == NULL) {
		myNums->printVals();
	} else {
		myNums->printVals(output);
	}

	myNums->cleanup();
}
