#ifndef IBP_H
#define IBP_H
#include <string>
#include <vector>

class IBP{
    public:
        int block_num;
        int block_size;
        std::vector<int> dbps;
        int getBlockNum();
        std::vector<int> getDBPS();
        IBP();
        IBP(int block_size);
        IBP(int block_num, int block_size);
        void setDBP(int index, int val);

};


#endif