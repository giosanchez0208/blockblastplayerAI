#include "my_blocks.h"
#include <random>
#include <iostream>
#include <ctime>

std::vector<Block> MyBlocks::blocks;
std::mt19937 MyBlocks::gen(static_cast<unsigned int>(std::time(nullptr)));

/*
    ⋆⁺｡˚⋆˙‧₊☽ ◯ ☾₊‧˙⋆˚｡⁺⋆
    No custom blocks in our implementation.
    We'll use pre-defined blocks only.
    ⋆⁺｡˚⋆˙‧₊☽ ◯ ☾₊‧˙⋆˚｡⁺⋆
*/

void MyBlocks::initializeBlocks() {

    // 1x1 BLOCKS
    // square 1x1
    blocks.emplace_back(
        "1\n"
    );

    // 1x2 BLOCKS
    // horizontal 1x2
    blocks.emplace_back(
        "11\n"
    );
    // vertical 1x2
    blocks.emplace_back(
        "1\n"
        "1\n"
    );
    
    // 2x2 BLOCKS
    // square 2x2
    blocks.emplace_back(
        "11\n"
        "11\n"
    );
    // L-piece 2x2
    blocks.emplace_back(
        "10\n"
        "11\n"
    );
    blocks.emplace_back(
        "01\n"
        "11\n"
    );
    blocks.emplace_back(
        "11\n"
        "01\n"
    );
    blocks.emplace_back(
        "11\n"
        "10\n"
    );

    // 1x3 BLOCKS
    // Horizontal 1x3
    blocks.emplace_back(
        "111\n"
    );
    // Vertical 1x3
    blocks.emplace_back(
        "1\n"
        "1\n"
        "1\n"
    );
    
    // 2x3 BLOCKS
    // L-piece 2x3
    blocks.emplace_back(
        "10\n"
        "10\n"
        "11\n"
    );
    blocks.emplace_back(
        "001\n"
        "111\n"
    );
    blocks.emplace_back(
        "11\n"
        "01\n"
        "01\n"
    );
    blocks.emplace_back(
        "111\n"
        "100\n"
    );
    // Reverse L-piece 2x3
    blocks.emplace_back(
        "01\n"
        "01\n"
        "11\n"
    );
    blocks.emplace_back(
        "111\n"
        "001\n"
    );
    blocks.emplace_back(
        "11\n"
        "10\n"
        "10\n"
    );
    blocks.emplace_back(
        "100\n"
        "111\n"
    );
    // T-piece 2x3
    blocks.emplace_back(
        "010\n"
        "111\n"
    );
    blocks.emplace_back(
        "10\n"
        "11\n"
        "10\n"
    );
    blocks.emplace_back(
        "111\n"
        "010\n"
    );
    blocks.emplace_back(
        "01\n"
        "11\n"
        "01\n"
    );
    // Rectangle 2x3
    blocks.emplace_back(
        "111\n"
        "111\n"
    );
    blocks.emplace_back(
        "11\n"
        "11\n"
        "11\n"
    );
    // Z-Piece 2x3
    blocks.emplace_back(
        "011\n"
        "110\n"
    );
    blocks.emplace_back(
        "01\n"
        "11\n"
        "10\n"
    );
    // S-Piece 2x3
    blocks.emplace_back(
        "110\n"
        "011\n"
    );
    blocks.emplace_back(
        "10\n"
        "11\n"
        "01\n"
    );

    // 3x3 BLOCKS
    // L-piece 3x3
    blocks.emplace_back(
        "100\n"
        "100\n"
        "111\n"
    );
    blocks.emplace_back(
        "001\n"
        "001\n"
        "111\n"
    );
    blocks.emplace_back(
        "111\n"
        "001\n"
        "001\n"
    );
    blocks.emplace_back(
        "111\n"
        "100\n"
        "100\n"
    );
    // Square 3x3
    blocks.emplace_back(
        "111\n"
        "111\n"
        "111\n"
    );

    // 1x4 BLOCKS
    blocks.emplace_back(
        "1111\n"
    );
    blocks.emplace_back(
        "1\n"
        "1\n"
        "1\n"
        "1\n"
    );

    // 1x5 BLOCKS
    blocks.emplace_back(
        "1111\n"
    );
    blocks.emplace_back(
        "1\n"
        "1\n"
        "1\n"
        "1\n"
        "1\n"
    );
}

Block MyBlocks::getRandomBlock() {
    if (blocks.empty()) {
        initializeBlocks();
    }

    // Uniform distribution for block indices
    std::uniform_int_distribution<> dis(0, blocks.size() - 1);
    return blocks[dis(gen)];
}