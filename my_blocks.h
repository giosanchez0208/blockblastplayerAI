#ifndef MY_BLOCKS_H
#define MY_BLOCKS_H

#include <vector>
#include <random>
#include "block.h"

class MyBlocks {
public:
    static void initializeBlocks();
    static Block getRandomBlock();

private:
    static std::vector<Block> blocks;
    static std::mt19937 gen; // for random numbers
};

#endif
