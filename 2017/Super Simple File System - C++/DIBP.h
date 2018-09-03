#ifndef DIBP_H
#define DIBP_H
#include <string>
#include <vector>
#include "IBP.h"

class DIBP{
    public:
        int getBlockNum();
        
        DIBP();
        DIBP(int block_size);
        DIBP(int block_num, int block_size);
        std::vector<IBP*> getIBPS();
        void makeIBP(int index, int block_num, int block_size);
        

        int block_num;
        std::vector<IBP*> ibps;

};





#endif