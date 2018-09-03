#ifndef FILE_H
#define FILE_H
#include <string>
#include <vector>
#include "IBP.h"
#include "DIBP.h"

class File{
	public:
		std::string filename;
		int filesize;
		int inode_block;
		int dbps[12];
		IBP* ibp_num;
		DIBP* dibp_num;

		File();
		File(int block_size);
		File(std::string filename, int block_size, int inode_block);
		std::string getFilename();
		int getSize();
		void addDBP(int index, int val);
		int returnInodeBlock();
		int returnDBPIndex(int index);
		IBP* returnIBPNum();
		DIBP* returnDIBPNum();
		void appendFilesize(int append);
		void setIBP(IBP* ibp);
		void setDIBP(DIBP* dibp);
};

#endif
