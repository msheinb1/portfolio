#include "Generate.h"
#include "LinkedList.h"
#include <iostream>

using namespace std;

	//Optimum page replacement
	// - have two counters. One counter counts the current page. Other keeps track of the highest index that the algorithm has searched for replacements (replaceCount).
	// - Have two arrays that are size of cache.
	// - One array is an int array, holds page #s.
	// - Other array is a bool array, holds data saying if the page is already found in future
	// - On page hit, push page to front. 
	// - On page miss, check to see if what replaceCount is pointing at is a page currently in the table.
	// - If so, set bool array associated with that page to true.
	// - Increment replaceCount, repeat until only one page bool count is false. Eject that page. Reset bool counters.

int optimumAux(int * workload, int cacheSize){
	int cache[cacheSize];
	bool future[cacheSize];
	int hits = 0;
	int cacheIndex = 0;
	bool isHit;
	int replaceCount = 0;
	int numTrue = 0;
	int replaceIndex = -1;
	Node * temp;

	for (int i = 0; i < cacheSize; i++) {
		cache[i] = -1;
	}

	for (int i = 0; i < 10000; i++) { //True Val = 10000
		isHit = false;
		numTrue = 0;
		replaceIndex = -1;
		for (int n = 0; n < cacheSize; n++) {
			future[n] = false;
		}

		for (int j = 0; j < cacheSize; j++) {
			if (cache[j] == workload[i]) {
				isHit = true;
				hits++;				
				break;
			}
		}
			if (isHit == false) {
				if (cacheIndex != cacheSize) {
					cache[cacheIndex] = workload[i];
					cacheIndex++;
				} else {
					replaceCount = i+1;

					while (numTrue < cacheSize-1 && replaceCount < 10000) {
						for (int k = 0; k < cacheSize; k++) {
							if (cache[k] == workload[replaceCount]) {
								if (future[k] == false) {
									future[k] = true;
									numTrue++;
								}
								break;
							}
						}
						replaceCount++;
					}

					for(int k = 0; k < cacheSize; k++) {
						if (future[k] == false) {
							replaceIndex = k;
							break;
						}
					}
					cache[replaceIndex] = workload[i];
					/*
					for (int j = 0; j < cacheSize; j++) {
						if (cache[j] == removed->data) {
							cache[j] = workload[i];
							break;
						}
					}
					*/
				}
			}
	}
	return hits;
}

void optimum(int * workload) {
	int temp = 0;
	cout << "Optimum Algorithm" << endl;
	for (int i = 5; i <= 100; i = i+5) {
		temp = optimumAux(workload, i);
		cout << temp << " ";
	}
	cout << endl;
}

/*
Least Recently Used page replacement
On page hit, push page to front.
On page miss, eject the page at the back of the Linked List, which will be the least recently used.
*/


int lruAux(int * workload, int cacheSize) {
	LinkedList * myList = new LinkedList();
	int cache[cacheSize];
	int hits = 0;
	int cacheIndex = 0;
	bool isHit;
	Node * temp;

	for (int i = 0; i < cacheSize; i++) {
		cache[i] = -1;
	}

	for (int i = 0; i < 10000; i++) { //True Val: 10000
		isHit = false;
		for (int j = 0; j < cacheSize; j++) {
			if (cache[j] == workload[i]) {
				isHit = true;
				hits++; 
				temp = myList->remove(workload[i]);
				myList->pushFront(temp);
				break;
			}
		}
			if (isHit == false) {
				if (cacheIndex != cacheSize) {
					cache[cacheIndex] = workload[i];
					temp = new Node(workload[i]);
					myList->pushFront(temp);
					cacheIndex++;
				} else {
					Node * removed = myList->remove();
					temp = new Node(workload[i]);
					myList->pushFront(temp);
					for (int j = 0; j < cacheSize; j++) {
						if (cache[j] == removed->data) {
							cache[j] = workload[i];
							break;
						}
					}
				}
			}
	}
	return hits;
}

void lru(int * workload) {
	int temp = 0;
	cout << "LRU Algorithm" << endl;
	for (int i = 5; i <= 100; i = i+5) {
		temp = lruAux(workload, i);
		cout << temp << " ";
	}
	cout << endl;
}

/*
FIFO page replacement
On page hit, do nothing besides increment the hit counter.
Keep track of the next page to be ejected with the cacheIndex variable.
On page miss, eject the contents of the cache at the index equivalent to cacheIndex variable.
*/

int FIFOAux(int * workload, int cacheSize) {
	int cache[cacheSize];
	int hits = 0;
	int cacheIndex = 0;
	bool isHit;

	for (int i = 0; i < cacheSize; i++) {
		cache[i] = -1;
	}

	for (int i = 0; i < 10000; i++) {
		isHit = false;
		//check for hit
		for (int j = 0; j < cacheSize; j++) {
			if (cache[j] == workload[i]) {
				hits++;				
				isHit = true;
				break;
			}
		}
		//replacement policy
		if (isHit == false) {
			cache[cacheIndex] = workload[i];
			cacheIndex++;
			cacheIndex = cacheIndex%cacheSize;
		}
	}
	return hits;
}

void FIFO(int * workload) {
	int temp = 0;
	cout << "FIFO Algorithm" << endl;
	for (int i = 5; i <= 100; i = i+5) {
		temp = FIFOAux(workload, i);
		cout << temp << " ";
	}
	cout << endl;
}

/*
Random page replacement
On page hit, do nothing besides increment the hit counter.
On page miss, randomly eject the contents of one page.
*/

int randomAux(int * workload, int cacheSize) {
	Generate * rng = new Generate(0, cacheSize, 10000);
	int cache[cacheSize];
	int hits = 0;
	int cacheIndex = 0;
	bool isHit;
	int counter = 0;
	int numMiss = 0;

	for (int i = 0; i < cacheSize; i++) {
		cache[i] = -1;
	}	

	for (int i = 0; i < 10000; i++) {
		isHit = false;
		//check for hit
		for (int j = 0; j < cacheSize; j++) {
			if (cache[j] == workload[i]) {
				hits++;				
				isHit = true;
				break;
			}
		}
		//replacement policy
		if (isHit == false) {
			if (numMiss < cacheSize) {
				numMiss++;
				for(int j = 0; j < cacheSize; j++) {
					if (cache[j] == -1) {
						cache[j] = workload[i];
						break;
					}
				}
			} else {
				//numMiss++;
				cacheIndex = rng->numArr[i];
				//cout << "Num Misses: " << numMiss << endl;
				//cout << "Cache Index: " << cacheIndex << endl;
				//cout << "Cache Contents Before Eviction: " << cache[cacheIndex] << endl;
				cache[cacheIndex] = workload[i];
				//cout << "Cache Contents After Eviction: " << cache[cacheIndex] << endl;
			}
		}
	}
	rng->cleanup();
	return hits;
}

void random(int * workload) {
	int temp = 0;
	cout << "Random Algorithm" << endl;
	for (int i = 5; i <= 100; i = i+5) {
		temp = randomAux(workload, i);
		cout << temp << " ";
	}
	cout << endl;
}

/*
Clock page replacement
Has two arrays, one array with the values in the cache, and one array of "clock bits".
The indexes of the cache and the clockBit arrays are related: cache[0] is the data associated with index 0, and clockbit[0] determines whether it had a hit recently.
The clockBit can be either 0 or 1.
The variable cacheIndex tracks the next potentially ejectable page.
On page hit, set the clock bit at the same index where the hit was found to 1 (recently used).
On page miss, check to see if the clock bit in the index with the same value as cacheIndex is 1. If it's 1, set it to 0, and move to the next spot in the cache.
If it's 0, eject the page.
*/

int clockAux(int * workload, int cacheSize) {
	int cache[cacheSize];
	int clockBit[cacheSize];
	int cacheIndex = 0;
	bool isHit;
	bool isReplaced;
	int hits = 0;
	int items = 0;

	for (int i = 0; i < cacheSize; i++) {
		cache[i] = -1;
	}

	for (int i = 0; i < cacheSize; i++) {
		clockBit[i] = 0;
	}
	for (int i = 0; i < 10000; i++) {
		isReplaced = false;
		isHit = false;
		//check for hit
		for (int j = 0; j < cacheSize; j++) {
			if (cache[j] == workload[i]) {
				clockBit[j] = 1;
				hits++;				
				isHit = true;
				break;
			}
		}
		//replacement policy
		if (isHit == false) {
			if (items != cacheSize) {
				cache[items] = workload[i];
				items++;
			} else {
				while (isReplaced == false) {
					if (clockBit[cacheIndex] == 0) {
						cache[cacheIndex] = workload[i];
						isReplaced = true;
					} else {
						clockBit[cacheIndex] = 0;
						cacheIndex++;
						cacheIndex = cacheIndex%cacheSize;
					}
				}
			}
		}
	}
	return hits;
}

void clock(int * workload) {
	int temp = 0;
	cout << "Clock Algorithm" << endl;
	for (int i = 5; i <= 100; i = i+5) {
		temp = clockAux(workload, i);
		cout << temp << " ";
	}
	cout << endl;
}

int main(int argc, char *argv[]) {
	int count = 0;

	//No Locality workload
	Generate * noLocal = new Generate(0, 99, 10000);
	int noLocality[10000];
	for (int i  = 0; i < 10000; i++) {
		noLocality[i] = noLocal->numArr[i];
	}
	noLocal->cleanup();
	cout << endl;
	//80-20 workload
	Generate * hot = new Generate(0, 19, 10000);
	Generate * cold = new Generate(20, 99, 10000);
	int hotCold[10000];
	Generate * myNums = new Generate(0, 9, 10000);
	for (int i = 0; i < 10000; i++) {
		if (myNums->numArr[i] < 8) {
			hotCold[i] = hot->numArr[i];
		} else {
			hotCold[i] = cold->numArr[i];
		}
	}
	hot->cleanup();
	cold->cleanup();
	myNums->cleanup();
	//Looping Workload
	int looping[10000];
	for (int i = 0; i < 10000; i++) {
		looping[i] = i%50;	
	}
	/*
	int tiny[10];
	tiny[0] = 0; //Miss - Cache: 0		Clock: >0 0 0
	tiny[1] = 1; //Miss - Cache: 0 1	Clock: >0 0 0
	tiny[2] = 2; //Miss - Cache: 0 1 2	Clock: >0 0 0
	tiny[3] = 0; //Hit			Clock: >1 0 0
	tiny[4] = 3; //Miss - Cache: 0 3 2	Clock: 0 >0 0
	tiny[5] = 1; //Miss - Cache: 0 1 2	Clock: 0 >0 0
	tiny[6] = 1; //Hit			Clock: 0 >1 0
	tiny[7] = 0; //Hit			Clock: 1 >1 0
	tiny[8] = 3; //Miss - Cache: 0 1 3	Clock: 1 0 >0
	tiny[9] = 2; //Miss - Cache: 0 1 2
	//Expected Hits: 3
	optimum(tiny);
	lru(tiny);
	FIFO(tiny);
	clock(tiny);
	*/
	
	cout << "No Locality Workload" << endl;
	optimum(noLocality);
	lru(noLocality);
	FIFO(noLocality);
	random(noLocality);
	clock(noLocality);
	cout << endl;

	cout << "Hot-Cold Workload" << endl;
	optimum(hotCold);
	lru(hotCold);
	FIFO(hotCold);
	random(hotCold);
	clock(hotCold);
	cout << endl;
	
	cout << "Looping Workload" << endl;
	optimum(looping);
	lru(looping);
	FIFO(looping);
	random(looping);
	clock(looping);
}
