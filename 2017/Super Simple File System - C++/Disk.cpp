#include "Disk.h"
#include <vector>
#include <iostream>
#include "File.h"

using namespace std;

/*
	This file deals with the construction and maintenance of the Disk.
	The disk holds the files, and free inodes.
	There is only one Disk.
*/


Disk::Disk(){
	
}

Disk::Disk(string diskname, int num_blocks, int block_size){
//	cout << "Disk constructor" << endl;
	this->diskname = diskname;
	this->num_blocks = num_blocks;
	this->block_size = block_size;
//	 cout << "Disk constructor 2" << endl;
//	this->free_blocks = vector<bool>((num_blocks-257), true);
	for(int i = 0; i < num_blocks-257; i++){
		this->free_blocks.push_back(true);
	}
//	cout << "Disk constructor 3" << endl;
	this->files = vector<File>(256, File(block_size));
//	cout << "Disk constructor 4" << endl;
	this->free_inodes = vector<bool>(256, true);
//	cout << "Disk constructor 5" << endl;
}


void Disk:: loadDisk(){
	
}


vector<File> Disk:: getFiles(){
	return this->files;
}

vector<bool> Disk:: getFreeBlocks(){
	return this->free_blocks;
}

void Disk:: setFree(int index, bool free){
	this->free_blocks[index] = free;
}

vector<bool> Disk:: getFreeInodes(){
	return this->free_inodes;
}

void Disk::setInode(int index, File file){
	this->files[index] = file;
	setFreeInode(index, false);
}

void Disk:: setFreeInode(int index, bool taken){
	this->free_inodes[index] = taken;
}

void Disk:: setDBP(int file_num, int index, int val){
	this->files[file_num].addDBP(index, val);
}

void Disk:: deleteFile(int index, int block_size){
	this->files[index] = File(block_size);
}

int Disk::getBlockSize(){
	return this->block_size;
}

//returns -1 if not found
int Disk::getFile(int block_number) {
	int index = -1;
	File temp;
	for (vector<File>::iterator it = files.begin() ; it != files.end(); ++it) {
		temp = *it;
		if (temp.inode_block == block_number) {
			index = it - files.begin();
		}
	}
	return index;
}
