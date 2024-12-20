#ifndef BOARD_STATE_H
#define BOARD_STATE_H

#include <cstdint>
#include <string>
#include "constants.h"
#include "block.h"

/*
    ⋆⁺｡˚⋆˙‧₊☽ ◯ ☾₊‧˙⋆˚｡⁺⋆
    Our *BOARD STATE* will be represented as a uintfast64_t data type.
    This is because a board state has exactly 64 spaces on an 8x8 grid.
    Each group seperated by an single quote (') will represent each row,
    from first to last respectively.
    ⋆⁺｡˚⋆˙‧₊☽ ◯ ☾₊‧˙⋆˚｡⁺⋆
*/

class BoardState {
private:
    uint_fast64_t boardState;

public:
    // constructor
    BoardState(uint_fast64_t boardState);
    BoardState(const std::string &boardString);
    BoardState();
    BoardState(const Block &block);

    // getter and setter
    uint_fast64_t getBoardState() const;
    void setBoardState(uint_fast64_t newBoardState);

    bool canInsert(BoardState boardStateToInsert);

    // insert
    bool canInsert(uint_fast64_t boardStateToInsert);
    BoardState insert(BoardState boardStateToInsert);
    BoardState insert(uint_fast64_t boardStateToInsert);

    // clear
    void clear();

    // shifting (will be useful for the simulated annealing part)
    // for clarification: up (^) down (v) left (<) right (>)
    // 1. decide if shifting is possible
    bool canShiftUp() const;
    bool canShiftDown() const;
    bool canShiftLeft() const;
    bool canShiftRight() const;

    // 2. perform shift. this is achieved with bitwise operations.
    BoardState shiftUp() const;
    BoardState shiftDown() const;
    BoardState shiftLeft() const;
    BoardState shiftRight() const;
    std::string toString() const;
    int countBits() const;
    bool canFitBlock(BoardState blockState);
    bool canFitBlock(const Block &block);
    int getBoardFitness() const;
};

#endif