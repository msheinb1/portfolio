#ifndef DISK_H
#define DISK_H
#include <string>
#include <vector>
#include "File.h"

class Disk{
	public:
		Disk();
		Disk(std::string diskname, int num_blocks, int block_size);
		void loadDisk();
		std::vector<File> getFiles();
		std::vector<bool> getFreeBlocks();
		void setFree(int index, bool free);
		std::vector<bool> getFreeInodes();
		void setInode(int index, File file);
		void setFreeInode(int index, bool taken);
		void setDBP(int file_num, int index, int val);
		int getBlockSize();
		void deleteFile(int index, int block_size);
		int getFile(int block_number);
		
 
		std::string diskname;
		int num_blocks;
		int block_size;
		//inode maps
		//free block list
		std::vector<bool> free_blocks;
	//	std::vector<int> inode_map;
		std::vector<File>files;
		std::vector<bool>free_inodes;
		
};

#endif



/**
 *	Disk structure should be as such: 
 *  First block should contain metadata about thedisk, like name, num_blocks, block_size, free block list, free inode list, etc.
 *  The next 256 blocks should contain the inodes. This is static to all disks
 * The remaining blocks will be allocated to files/inodes as needed
 **/