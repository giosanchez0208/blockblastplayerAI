#include "board_state.h"
#include "block.h"
#include <string>
#include <math.h>
#include <iostream>

// constructor
BoardState::BoardState(uint_fast64_t boardState) {
    this->boardState = boardState;
    this->clear();
}
BoardState::BoardState(const Block& block) {
    this->boardState = block.getBlock();
    this->clear();
}
BoardState::BoardState(const std::string& boardString) {
    this->boardState = 0;
    int bitPos = 0;
    for (char c : boardString) {
        if (c == ' ' || c == '\n') continue; // ignore whitespace
        if (c == 'X' || c == '1') {
            this->boardState |= (1ULL << bitPos);
        } else if (c != 'O' && c != '0') {

        }
        bitPos++;
    }
    this->clear();
}
BoardState::BoardState() {
    this->boardState = 0;
    this->clear();
}

// getter and setter
uint_fast64_t BoardState::getBoardState() const {
    return this->boardState;
}

void BoardState::setBoardState(uint_fast64_t newBoardState) {
    this->boardState = newBoardState;
}

bool BoardState::canInsert(BoardState boardStateToInsert) {
    return (this->boardState & boardStateToInsert.getBoardState()) == 0;
}
bool BoardState::canInsert(uint_fast64_t boardStateToInsert) {
    return (this->boardState & boardStateToInsert) == 0;
}

BoardState BoardState::insert(BoardState boardStateToInsert) {
    BoardState insertedState(boardState | boardStateToInsert.getBoardState());
    insertedState.clear();
    return insertedState;
}
BoardState BoardState::insert(uint_fast64_t boardStateToInsert) {
    BoardState insertedState(boardState | boardStateToInsert);
    insertedState.clear();
    return insertedState;
}

// clear
void BoardState::clear() {
    // we will store all the full rows and cols data here
    uint_fast64_t fullRowsAndCols = 0;
    // then we will check which rows/cols are full before clearing
    if ((this->boardState & ROW1) == ROW1) fullRowsAndCols |= ROW1;
    if ((this->boardState & ROW2) == ROW2) fullRowsAndCols |= ROW2;
    if ((this->boardState & ROW3) == ROW3) fullRowsAndCols |= ROW3;
    if ((this->boardState & ROW4) == ROW4) fullRowsAndCols |= ROW4;
    if ((this->boardState & ROW5) == ROW5) fullRowsAndCols |= ROW5;
    if ((this->boardState & ROW6) == ROW6) fullRowsAndCols |= ROW6;
    if ((this->boardState & ROW7) == ROW7) fullRowsAndCols |= ROW7;
    if ((this->boardState & ROW8) == ROW8) fullRowsAndCols |= ROW8;
    if ((this->boardState & COL1) == COL1) fullRowsAndCols |= COL1;
    if ((this->boardState & COL2) == COL2) fullRowsAndCols |= COL2;
    if ((this->boardState & COL3) == COL3) fullRowsAndCols |= COL3;
    if ((this->boardState & COL4) == COL4) fullRowsAndCols |= COL4;
    if ((this->boardState & COL5) == COL5) fullRowsAndCols |= COL5;
    if ((this->boardState & COL6) == COL6) fullRowsAndCols |= COL6;
    if ((this->boardState & COL7) == COL7) fullRowsAndCols |= COL7;
    if ((this->boardState & COL8) == COL8) fullRowsAndCols |= COL8;
    // the intersection of not(fullRowsAndCols) and the board state
    // will give us the cleared board state.
    this->boardState &= ~fullRowsAndCols;
}

// shifting (will be useful for the simulated annealing part)
// for clarification: up (^) down (v) left (<) right (>)
// 1. decide if shifting is possible
bool BoardState::canShiftUp() const {
    return (this->boardState & ROW8) == 0;
}

bool BoardState::canShiftDown() const {
    return (this->boardState & ROW1) == 0;
}

bool BoardState::canShiftLeft() const {
    return (this->boardState & COL1) == 0;
}

bool BoardState::canShiftRight() const {
    return (this->boardState & COL8) == 0;
}

// 2. perform shift. this is achieved with bitwise operations.
BoardState BoardState::shiftUp() const {
    return BoardState(boardState >> 8);
}

BoardState BoardState::shiftDown() const {
    return BoardState(boardState << 8);
}

BoardState BoardState::shiftLeft() const {
    return BoardState((boardState >> 1));
}

BoardState BoardState::shiftRight() const {
    return BoardState((boardState << 1));
}


// canFitBlock checks if we can fit a block into a board anywhere
// (1) align block to top left (2) move in s motion checking everywhere in board
// (3) if can't be inserted anywhere, false, otherwise true.
bool BoardState::canFitBlock(BoardState blockState) {
    bool inserted = false;
    // align the block to the top-left corner (shift up, then left)
    while (blockState.canShiftUp()) blockState = blockState.shiftUp();
    while (blockState.canShiftLeft()) blockState = blockState.shiftLeft();
    //check in s motion if block can fit (all the way right, down, then all the way left, down)
    do {
        while (blockState.canShiftRight()) {
            blockState = blockState.shiftRight();
            if (canInsert(blockState.getBoardState())) {
                inserted = true;
                break;
            }
            if (blockState.canShiftDown()) {
                blockState = blockState.shiftDown();
            } else {
                break;
            }
        }
        if (!inserted) {
            while (blockState.canShiftLeft()) {
                blockState = blockState.shiftLeft();
                if (canInsert(blockState.getBoardState())) {
                    inserted = true;
                    break;
                }
                if (blockState.canShiftDown()) {
                    blockState = blockState.shiftDown();
                } else {
                    break;
                }
            }
        }
    } while (!inserted && blockState.canShiftDown());
    return inserted;
}

// the fitness of the board is determined by "regularity". in this case, transitions.
// it considers how patterns align across multiple scales. more regular spaces encourage block fitting.
// a higher fitness score indicates fewer transitions and a more compact, aligned board state.
// here, i designed three types of transitions, WIDE, NARROW, and THIN.
// we encourage lack of transitions in larger spaces while punishing irregular solutions

int BoardState::getBoardFitness() const {
    uint_fast64_t boardCopy = this->boardState;

    int wideConsistency = 0;   
    int narrowConsistency = 0; 
    int thinConsistency = 0;  

    // wide comb
    for (int row = 0; row < 7; row++) { 
        for (int col = 0; col < 6; col++) { 
            uint_fast64_t group1 = (boardCopy >> (row * 8 + col)) & 0b111;
            uint_fast64_t group2 = (boardCopy >> (row * 8 + col + 1)) & 0b111;
            if (group1 == group2) wideConsistency++;
        }
    }
    for (int col = 0; col < 7; col++) { 
        for (int row = 0; row < 6; row++) { 
            uint_fast64_t group1 = (boardCopy >> (row * 8 + col)) & 0b111;
            uint_fast64_t group2 = (boardCopy >> ((row + 1) * 8 + col)) & 0b111;
            if (group1 == group2) wideConsistency++;
        }
    }

    // narrow comb
    for (int row = 0; row < 7; row++) { 
        for (int col = 0; col < 7; col++) { 
            uint_fast64_t group1 = (boardCopy >> (row * 8 + col)) & 0b11;
            uint_fast64_t group2 = (boardCopy >> (row * 8 + col + 1)) & 0b11;
            if (group1 == group2) narrowConsistency++;
        }
    }
    for (int col = 0; col < 7; col++) { 
        for (int row = 0; row < 7; row++) { 
            uint_fast64_t group1 = (boardCopy >> (row * 8 + col)) & 0b11;
            uint_fast64_t group2 = (boardCopy >> ((row + 1) * 8 + col)) & 0b11;
            if (group1 == group2) narrowConsistency++;
        }
    }

    // thin comb
    for (int row = 0; row < 7; row++) { 
        for (int col = 0; col < 8; col++) { 
            bool bit1 = (boardCopy >> (row * 8 + col)) & 1;
            bool bit2 = (boardCopy >> (row * 8 + col + 1)) & 1;
            if (bit1 == bit2) thinConsistency++;
        }
    }
    for (int col = 0; col < 7; col++) { 
        for (int row = 0; row < 8; row++) { 
            bool bit1 = (boardCopy >> (row * 8 + col)) & 1;
            bool bit2 = (boardCopy >> ((row + 1) * 8 + col)) & 1;
            if (bit1 == bit2) thinConsistency++;
        }
    }

    // fitness approaches 1000000 if variances across scales are similar
    int scaledWideConsistency = (wideConsistency * 1000000) / 84;
    int scaledNarrowConsistency = (narrowConsistency * 1000000) / 98;
    int scaledThinConsistency = (thinConsistency * 1000000) / 112;

    // fitness
    int fitness = 1000000; //arbitrary

    // punish irregularity
    int irregularity = abs(scaledWideConsistency - scaledNarrowConsistency) + 
                       abs(scaledNarrowConsistency - scaledThinConsistency);
    fitness = fitness - irregularity;

    // punish larger inconsistencies more harshly
    fitness = double (fitness * pow(wideConsistency, 4) / pow(112,4)
                              * pow(narrowConsistency, 2) / pow(98, 2)
                              * pow(thinConsistency, 1) / pow(84,1));

    // when board less than halfway full, only focus on regularity and consistency
    // when board more than halfway full, consider reducing blocks
    if(countBits()>4)
        fitness = double(fitness * pow(64-countBits(),2)/pow(64,2));

    return fitness;
}


int BoardState::countBits() const {
    uint_fast64_t n = this->boardState;
    int count = 0;
    
    // Brian Kernighan's Algorithm
    while (n) {
        n &= (n - 1);
        count++;
    }
    
    return count;
}

// convert boardState to a string
std::string BoardState::toString() const {
    std::string boardAsString;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            int bitPosition = row * 8 + col;
            if (boardState & (1ULL << bitPosition)) {
                boardAsString += "[]";  // block
            } else {
                boardAsString += "_'";  // space
            }
        }
        boardAsString += "\n";
    }
    return boardAsString;
}
