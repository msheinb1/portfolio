#include "Timer.h"
#include "Sorter.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <fstream>

using namespace std;

//Compares two int values and returns the result 

int cmprfc(const void * a, const void * b)
	{
	   return ( *(int*)a - *(int*)b );
	}

//Uses switch statements and getopt method to input and sort up to 1,000,000 numbers from 0 to 1,000,000. The input numbers can either be input manually or parsed from an input file.

int main(int argc, char *argv[]) {
	Timer * timeMe = new Timer();
	int command;
	int numInts = -1;
	int counter = -1;
	int minInt = 1;
	int maxInt = 255;
	char * input = NULL;
	char * output = NULL;
	char * countFile = NULL;

	int temp;
	char *endptr;

	int check;
	while ((command = getopt(argc, argv, "un:m:M:i:o:c:")) != -1) {
		switch (command) {
			case 'u':
				fprintf(stderr, "Usage: prog1sorter -u -n <num-integers> -m <min int> -M <max-int> -i <input-file-name> -o <output-filename> -c <count-file-name>\n");
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
			case 'i':
				input = optarg;
				break;
			case 'o':
				output = optarg;
				break;
			case 'c':
				countFile = optarg;
				break;
			case '?':
				if (((*optarg) == 'n') || ((*optarg) == 'm') || ((*optarg) == 'M') || ((*optarg) = 'i') || ((*optarg) == 'o') || ((*optarg) == 'c')) {
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


//This block lets you manually input numbers to be sorted.

	int * myNums;
	if (input == NULL) {
		cin >> counter;
		if (counter <= 0) {
			fprintf(stderr, "You tried to make an array smaller than 1.\n");
			exit(1);
		}
		if (numInts != -1 && numInts != counter) {
			fprintf(stderr, "number of ints specified by -n flag does not match number of values to be entered. Exiting\n");
			exit(1);
		}

		myNums = (int*) malloc(counter*sizeof(int));
		if (myNums == NULL) {
			fprintf(stderr, "Error with generating array.\n");
			exit(1);
		}

		for (int i = 0; i < counter; i++) {
			while (!(cin >> temp)) {
	   			 cin.clear(); //clear bad input flag
	   			 cin.ignore(1000, '\n'); //discard input
	   			 cout << "Invalid input; please re-enter.\n";
			}
			if (temp < minInt || temp > maxInt) {
				fprintf(stderr, "You entered an integer out of bounds\n");
				exit(1);
			}
			myNums[i] = temp;
		}
	} else {

//This block takes in an input file for the numbers to be sorted.

		string tempVal;
		int tempCounter = 0;

		ifstream infile (input);
		if (infile.is_open()) {
			while (! infile.eof()) {
				getline (infile, tempVal);
				temp = atoi(tempVal.c_str()); 

				if (counter == -1) {
					counter = temp;
					myNums = (int*) malloc(counter*sizeof(int));
					if (myNums == NULL) {
						fprintf(stderr, "Error with generating array.\n");
						exit(1);
					}
					if (numInts != -1 && numInts != counter) {
						fprintf(stderr, "number of ints specified by -n flag does not match number of values to be entered. Exiting\n");
						exit(1);
					}
				} else {
					if (temp < minInt || temp > maxInt) {
						fprintf(stderr, "You entered an integer out of bounds\n");
						exit(1);
					}
					myNums[tempCounter] = temp;
					tempCounter++;
				}
				if(tempCounter == counter) {
					break;
				}
			}
			if (tempCounter < counter) {
				counter = tempCounter;
			}
		}
		infile.close();
	}

/*
Sorts the numbers in the sorter object.
It gets the username of the user, and counts the number of times the int representing the char of each letter in the username appears in the initial array of numbers.
It then either writes the result to the terminal or the file.
*/

	timeMe->setStart();
	cout << endl;

	Sorter * mySorter = new Sorter(myNums, counter);	
	free(myNums);
	qsort(mySorter->getArr(), mySorter->getArrSize(), sizeof(int), cmprfc);

	char * userID = getlogin();
	if (userID == NULL) {
		fprintf(stderr, "Username not detected");
		exit(1);
	}

	int numChars = sizeof(userID)/sizeof(char);
	int timesFound = 0;
	if (output == NULL && countFile == NULL) {
		for (int i = 0; i < numChars; i++) {
			timesFound = mySorter->numInArr((int) userID[i]);
			cout << userID[i] << "\t" << (int) userID[i] << "\t" << timesFound << endl;
		}
	} else if (output != NULL && countFile == NULL) {
		FILE * outfile = fopen(output, "w");
		if (outfile == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			exit(1);
		}

		for (int i = 0; i < numChars; i++) {
			timesFound = mySorter->numInArr((int) userID[i]);
			fprintf(outfile, "%c\t%d\t%d\n", userID[i], (int) userID[i], timesFound);
		}
		fclose(outfile);
	} else if (output == NULL && countFile != NULL) {
		FILE * outfile = fopen(countFile, "w");
		if (outfile == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			exit(1);
		}

		for (int i = 0; i < numChars; i++) {
			timesFound = mySorter->numInArr((int) userID[i]);
			fprintf(outfile, "%c\t%d\t%d\n", userID[i], (int) userID[i], timesFound);
		}
		fclose(outfile);
	} else {
		FILE * outfile1 = fopen(output, "w");
		FILE * outfile2 = fopen(countFile, "w");
		if (outfile1 == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			exit(1);
		}
		if (outfile2 == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			exit(1);
		}

		for (int i = 0; i < numChars; i++) {
			timesFound = mySorter->numInArr((int) userID[i]);
			fprintf(outfile1, "%c\t%d\t%d\n", userID[i], (int) userID[i], timesFound);
			fprintf(outfile2, "%c\t%d\t%d\n", userID[i], (int) userID[i], timesFound);
		}
		fclose(outfile1);
		fclose(outfile2);
	}
	timeMe->setEnd();
	mySorter->cleanup();
	timeMe->printTime();
}
