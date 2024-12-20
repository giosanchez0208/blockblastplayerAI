#include "move.h"
#include "board_state.h"
#include "block.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

MoveSet::MoveSet(const BoardState& initialBoard, const Block& block1, const Block& block2, const Block& block3) {
  this->startBoard = initialBoard;
  this->block1 = block1.getBlock();
  this->block2 = block2.getBlock();
  this->block3 = block3.getBlock();
  this->move1 = BoardState(block1);
  this->move2 = BoardState(block2);
  this->move3 = BoardState(block3);
  this->finalBoard = startBoard;
}

MoveSet::MoveSet(const MoveSet& other) {
    this->startBoard = other.startBoard;
    this->move1 = other.move1;
    this->move2 = other.move2;
    this->move3 = other.move3;
    this->finalBoard = other.finalBoard;
}

// implementation: if canPerformMoveSet(), getFinalMoveSet

bool MoveSet::canPerformMoveSet() {

  this->finalBoard = startBoard;
  if (finalBoard.canInsert(move1)){
    this->finalBoard = finalBoard.insert(move1);
  } else {
    this->finalBoard = startBoard;
    return false;
  }
  if (finalBoard.canInsert(move2)){
    this->finalBoard = finalBoard.insert(move2);
  } else {
    this->finalBoard = startBoard;
    return false;
  }
    if (finalBoard.canInsert(move3)){
    this->finalBoard = finalBoard.insert(move3);
  } else {
    this->finalBoard = startBoard;
    return false;
  }
  return true;
}

BoardState MoveSet::getFinalMoveSet() {
  return this->finalBoard;
}

// simulated annealing part

void MoveSet::findValidSolution() {
    int maxAttempts = 2000000;
    int attempts = 0;

    const std::vector<std::vector<Block>> blockPermutations{
        {Block(block1), Block(block2), Block(block3)},
        {Block(block1), Block(block3), Block(block2)},
        {Block(block2), Block(block1), Block(block3)},
        {Block(block2), Block(block3), Block(block1)},
        {Block(block3), Block(block1), Block(block2)},
        {Block(block3), Block(block2), Block(block1)}
    };

    for (const auto& perm : blockPermutations) {
        attempts = 0;
        *this = MoveSet(startBoard, perm[0], perm[1], perm[2]);
        while (!this->canPerformMoveSet() && attempts < maxAttempts) {
            this->perturb();
            attempts++;
        }
        if(this->canPerformMoveSet()){
            return;
        }
    }

  if (attempts >= maxAttempts) {
    throw std::runtime_error("Unable to find a valid solution after maximum attempts.");
  }
}


void MoveSet::perturb() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> moveTypeDistrib(0, 2);
    std::uniform_int_distribution<> moveDirectionDistrib(0, 3);

    bool perturbed = false;
    while (!perturbed) {

        int moveType = moveTypeDistrib(gen);
        int moveDirection = moveDirectionDistrib(gen);
        BoardState currentMove;

        switch (moveType) {
            case 0: currentMove = this->move1; break;
            case 1: currentMove = this->move2; break;
            case 2: currentMove = this->move3; break;
        }

        BoardState newMove = currentMove;
        switch (moveDirection) {
            case 0:  // up
                if (currentMove.canShiftUp()) {
                    newMove = currentMove.shiftUp();
                    perturbed = true;
                }
                break;
            case 1:  // down
                if (currentMove.canShiftDown()) {
                    newMove = currentMove.shiftDown();
                    perturbed = true;
                }
                break;
            case 2:  // left
                if (currentMove.canShiftLeft()) {
                    newMove = currentMove.shiftLeft();
                    perturbed = true;
                }
                break;
            case 3:  // right
                if (currentMove.canShiftRight()) {
                    newMove = currentMove.shiftRight();
                    perturbed = true;
                }
                break;
        }

        // update the specific move
        if (perturbed) {
            switch (moveType) {
                case 0: this->move1 = newMove; break;
                case 1: this->move2 = newMove; break;
                case 2: this->move3 = newMove; break;
            }
        }
    }
}


void MoveSet::anneal() {
    
    const std::vector<std::vector<Block>> blockPermutations{
        {Block(block1), Block(block2), Block(block3)},
        {Block(block1), Block(block3), Block(block2)},
        {Block(block2), Block(block1), Block(block3)},
        {Block(block2), Block(block3), Block(block1)},
        {Block(block3), Block(block1), Block(block2)},
        {Block(block3), Block(block2), Block(block1)}
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> probDist(0.0, 1.0);

    MoveSet bestMoveSet = *this;
    int bestScore = std::numeric_limits<int>::min();
    bool anySuccess = false;

    for (const auto& perm : blockPermutations) {
        MoveSet currentMoveSet(startBoard, perm[0], perm[1], perm[2]);
        
        // hybrid cooling temperature
        double temperature = 100.0;
        const double minTemperature = 0.1;
        
        // hybrid cooling rates
        double fastCoolingRate = 0.90;
        double slowCoolingRate = 0.99;
        double baseCoolingRate = 0.95;

        int maxIterations = 2000;
        int iterationCount = 0;
        int noImprovementCount = 0;
        const int MAX_NO_IMPROVEMENT = 500;

        currentMoveSet.findValidSolution();
        if (!currentMoveSet.canPerformMoveSet()) {
            continue;
        }

        int currentScore = currentMoveSet.getFinalMoveSet().getBoardFitness();
        int permBestScore = currentScore;
        MoveSet currentBestMoveSet = currentMoveSet;

        while (temperature > minTemperature && 
               iterationCount < maxIterations && 
               noImprovementCount < MAX_NO_IMPROVEMENT) {
            
            MoveSet neighbor = currentMoveSet;
            neighbor.perturb();

            if (neighbor.canPerformMoveSet()) {
                int neighborScore = neighbor.getFinalMoveSet().getBoardFitness();
                int deltaE = neighborScore - currentScore;

                // temperature accept with probability
                bool accept = deltaE > 0 || 
                    probDist(gen) < std::exp(deltaE / temperature);

                if (accept) {
                    currentMoveSet = neighbor;
                    currentScore = neighborScore;

                    // Update best solution tracking
                    if (neighborScore > permBestScore) {
                        currentBestMoveSet = neighbor;
                        permBestScore = neighborScore;
                        noImprovementCount = 0;
                    } else {
                        noImprovementCount++;
                    }
                } else {
                    noImprovementCount++;
                }
            }

            // hybrid cooling logic
            double acceptanceRatio = static_cast<double>(noImprovementCount) / iterationCount;
            if (acceptanceRatio > 0.7) {
                temperature *= fastCoolingRate;
            } else if (acceptanceRatio < 0.2) {
                temperature *= slowCoolingRate;  
            } else {
                temperature *= baseCoolingRate;
            }

            iterationCount++;

            // early termination if no significant improvement
            if (noImprovementCount > MAX_NO_IMPROVEMENT / 2) {
                break;
            }
        }

        // update global best solution
        if (currentBestMoveSet.canPerformMoveSet() && permBestScore > bestScore) {
            bestMoveSet = currentBestMoveSet;
            bestScore = permBestScore;
            anySuccess = true;
        }
    }

    // throw only if absolutely no solution found
    if (!anySuccess) {
        throw std::runtime_error("No valid solution found during annealing");
    }

    *this = bestMoveSet;
}

// simplified initial temperature calculation
double MoveSet::calculateInitialTemperature() {
    const int SAMPLE_SIZE = 50;  // Reduced sample size
    std::vector<double> deltaCosts;
    std::random_device rd;
    std::mt19937 gen(rd());

    MoveSet currentState = *this;
    
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        MoveSet neighbor = currentState;
        neighbor.perturb();
        
        if (neighbor.canPerformMoveSet() && currentState.canPerformMoveSet()) {
            int delta = neighbor.getFinalMoveSet().getBoardFitness() - 
                       currentState.getFinalMoveSet().getBoardFitness();
            deltaCosts.push_back(std::abs(delta));
        }
    }
    
    return deltaCosts.empty() ? 100.0 : 
        -std::accumulate(deltaCosts.begin(), deltaCosts.end(), 0.0) / 
        (deltaCosts.size() * std::log(0.8));
}

std::string MoveSet::toString() const {

    std::ostringstream oss;
    std::cout << std::endl;

    oss << "Move 1: \n" << this->move1.toString() << "\n";
    oss << "Move 2: \n" << this->move2.toString() << "\n";
    oss << "Move 3: \n" << this->move3.toString() << "\n";
    oss << "Initial board state: \n" << startBoard.toString() << "\n\n";
    oss << "Final board state: \n" << finalBoard.toString() << "\n\n";
    
    return oss.str();
}