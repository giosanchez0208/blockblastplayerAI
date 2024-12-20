#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <cstdint>

class Block {
private:
    uint_fast64_t block;

public:
    Block(uint_fast64_t block = 0); 
    Block(const std::string& blockStr);

    // Getters and Setters
    uint_fast64_t getBlock() const;
    void setBlockState(uint_fast64_t newBlock);

    // Core methods
    static uint_fast64_t boardify(const std::string& blockStr);
    std::string toCroppedString() const;
    std::string toString() const;
};

#endif