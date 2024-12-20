#include <algorithm>
#include <sstream>
#include "constants.h"
#include "block.h"

/*
    ⋆⁺｡˚⋆˙‧₊☽ ◯ ☾₊‧˙⋆˚｡⁺⋆
    When we parse a block string e.g.
    "
    100
    100 for an L piece,
    111
    "
    we will immediately convert it into a block on an
    8x8 board upon initialization
    ⋆⁺｡˚⋆˙‧₊☽ ◯ ☾₊‧˙⋆˚｡⁺⋆
*/

// constructor that takes a uint_fast64_t value
Block::Block(uint_fast64_t block) : block(block) {}
// constructor that takes a string
Block::Block(const std::string& blockStr) {
    this->block = boardify(blockStr);
}

uint_fast64_t Block::getBlock() const {
    return this->block;
}

void Block::setBlockState(uint_fast64_t newBlock) {
    this->block = newBlock;
}

// boardify()) converts our string to a block on an 8x8 board
uint_fast64_t Block::boardify(const std::string& blockStr) {
    uint_fast64_t result = 0;
    std::istringstream ss(blockStr);
    std::string line;
    int currentRow = 0;
    while (std::getline(ss, line) && currentRow < 8) {
        // remove whitespace
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        line.resize(8, '0');
        for (int col = 0; col < 8; ++col) {
            if (line[col] == '1') {
                result |= (1ULL << (currentRow * 8 + col)); 
            }
        }

        ++currentRow;
    }
    // continue adding rows until we have 8 rows
    while (currentRow < 8) {
        ++currentRow;
    }
    return result;
}

// crop method will (1) check for blank rows/cols then (2) remove those and finally 
// (3) returns the output as a string such as those we used to define our blocks
std::string Block::toCroppedString() const {
    // determine the bounding box of the block
    int minRow = 8, maxRow = -1;
    int minCol = 8, maxCol = -1;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (block & (1ULL << (row * 8 + col))) {
                minRow = std::min(minRow, row);
                maxRow = std::max(maxRow, row);
                minCol = std::min(minCol, col);
                maxCol = std::max(maxCol, col);
            }
        }
    }

    // create the cropped representation
    std::string croppedBlock;
    for (int row = minRow; row <= maxRow; ++row) {
        for (int col = minCol; col <= maxCol; ++col) {
            if (block & (1ULL << (row * 8 + col))) {
                croppedBlock += "[]";  // block
            } else {
                croppedBlock += "_'";  // space
            }
        }
        croppedBlock += "\n";
    }
    return croppedBlock;
}


// toString will just print the board as a string.
std::string Block::toString() const {
    // bounding box of the piece
    int minRow = 8, maxRow = -1;
    int minCol = 8, maxCol = -1;
    
    // scan to find the actual occupied area
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (block & (1ULL << ((7 - row) * 8 + (7 - col)))) {
                minRow = std::min(minRow, row);
                maxRow = std::max(maxRow, row);
                minCol = std::min(minCol, col);
                maxCol = std::max(maxCol, col);
            }
        }
    }

    // build the string representation
    std::stringstream ss;
    for (int row = minRow; row <= maxRow; ++row) {
        for (int col = minCol; col <= maxCol; ++col) {
            ss << ((block & (1ULL << ((7 - row) * 8 + (7 - col)))) ? '1' : '0');
        }
        ss << '\n';
    }
    return ss.str();
}
