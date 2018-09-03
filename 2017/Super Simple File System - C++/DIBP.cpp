#include <string>
#include <vector>
#include "IBP.h"
#include "DIBP.h"

/*
	This file deals with the construction and maintenance of Double Indirect Block Pointers.
	Double Indirect Block Pointers hold a vector of Indirect Block Pointers.
*/

using namespace std;

DIBP:: DIBP(){}

DIBP:: DIBP(int block_size){
    this->block_num = -1;
    this->ibps = vector<IBP*>(block_size/4, new IBP(block_size));
    for(int i = 0; i < block_size/4; i++){
        this->ibps[i] = new IBP(block_size);
    }
}

DIBP::DIBP(int block_num, int block_size){
    this->block_num = block_num;
    this->ibps = vector<IBP*>(block_size/4, new IBP(block_size));
    
}

void DIBP::makeIBP(int index, int block_num, int block_size){
    this->ibps[index] = new IBP(block_num, block_size);
}

vector<IBP*> DIBP::getIBPS(){
    return this->ibps;
}

int DIBP::getBlockNum(){
    return block_num;
}

