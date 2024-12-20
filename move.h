#ifndef BLOCK_BLAST_MOVE_H
#define BLOCK_BLAST_MOVE_H

#include "board_state.h"
#include "block.h"
#include <random>
#include <cmath>

class MoveSet {
private:
    BoardState startBoard;
    BoardState finalBoard;
    BoardState move1;
    BoardState move2;
    BoardState move3;
    std::mt19937 gen;

public:
    Block block1;
    Block block2;
    Block block3;
    MoveSet(const BoardState& initialBoard, const Block& block1, const Block& block2, const Block& block3);
    MoveSet(const MoveSet& other);
    bool canPerformMoveSet();
    BoardState getFinalMoveSet();
    void findValidSolution();
    void perturb();
    void anneal();
    double calculateInitialTemperature();
    std::string toRowString(const BoardState &board);
    std::string toString() const;
    std::vector<std::string> splitIntoRows(const std::string &boardStr) const;
};

#endif
