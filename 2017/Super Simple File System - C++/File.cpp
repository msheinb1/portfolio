#include "File.h"
#include "IBP.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;

/*
	This file deals with the construction and maintenance of Files.
	The File points to one Indirect Block pointer and one Double Indirect Block Pointer.
*/


File:: File(){
	filename = "***";
	filesize = 0;
}

File::File(int block_size){
	filename = "***";
	filesize = 0;
	this->inode_block = -1;
	for(int i = 0; i < 12; i++){
		this->dbps[i] = -1;
	}

	this->ibp_num = new IBP(block_size);
	this->dibp_num = new DIBP(block_size);

}

File::File(string filename, int block_size, int inode_block){
	this->filename = filename;
	this->filesize = 0;
	this->inode_block = inode_block;
	for(int i = 0; i < 12; i++){
		this->dbps[i] = -1;
	}
	this->ibp_num = new IBP(block_size);
	this->dibp_num = new DIBP(block_size);
}

string File:: getFilename(){
	return this->filename;
}

void File:: addDBP(int index, int val){
	this->dbps[index] = val;
}

int File:: getSize(){
	return this->filesize;
}

int File::returnInodeBlock(){
	return inode_block;
}

int File::returnDBPIndex(int index){
	return dbps[index];
}

IBP* File::returnIBPNum(){
	return ibp_num;
}

DIBP* File::returnDIBPNum(){
	return dibp_num;
}

void File:: appendFilesize(int append){
	this->filesize += append;
}

void File:: setIBP(IBP* ibp){
	this->ibp_num = ibp;
}

void File:: setDIBP(DIBP* dibp){
	this->dibp_num = dibp;
}
