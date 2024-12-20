#include "move.h"
#include "board_state.h"
#include "my_blocks.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <limits>
#include <iomanip>
#include <chrono>

/*
g++ -g -o BlockBlastWithSA main.cpp my_blocks.cpp move.cpp board_state.cpp block.cpp -std=c++11
./BlockBlastWithSA
*/

void clearTerminal() {
    // ANSI escape code to clear the terminal screen
    std::cout << "\033[2J\033[1;1H";
}

int main() {
    MyBlocks myBlocks = MyBlocks();
    BoardState startBoard = BoardState(0);
    int score = 0;
    int oldfitnessscore = 1;

    while (true) {
        Block block1 = myBlocks.getRandomBlock();
        Block block2 = myBlocks.getRandomBlock();
        Block block3 = myBlocks.getRandomBlock();

        std::cout << "\nROUND:" << score << std::endl;
        std::cout << "\nSTART BOARD: \n";
        std::cout << startBoard.toString();

        std::cout << "\nBLOCKS: \n";
        std::cout << block1.toCroppedString() << "\n";
        std::cout << block2.toCroppedString() << "\n";
        std::cout << block3.toCroppedString() << "\n";
        
        // pressing enter proceeds to next round
        std::cout << "\n\nPress Enter to Anneal...\n";
        std::cin.ignore(); 
        clearTerminal();

        // start measuring time before annealing
        auto startTime = std::chrono::high_resolution_clock::now();
        
        MoveSet bestMoveSet(startBoard, block1, block2, block3);

        try {
            bestMoveSet.anneal();
        } catch (const std::runtime_error& e) {
            std::cerr << "Error during annealing: " << e.what() << "\n";
            break;
        }

        // end measuring time after annealing
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = endTime - startTime;
        
        // if no valid moves are found, stop the game
        if (!bestMoveSet.canPerformMoveSet()) {
            std::cout << "Game Over!\n";
            break;
        }

        std::cout << "\nFINAL MOVE SET: \n";
        std::cout << bestMoveSet.toString();
        
        // increment score
        score++;

        // print the time taken for the annealing process
        std::cout << "Annealing time: " 
                << std::fixed << std::setprecision(7) 
                << duration.count() << " seconds" << std::endl;

        std::cout << "Old Fitness Score: " 
                          << std::fixed << std::setprecision(7) 
                          << oldfitnessscore / 1000000.0 
                          << std::endl;

        oldfitnessscore = bestMoveSet.getFinalMoveSet().getBoardFitness();
        std::cout << "New Fitness Score: " 
                          << std::fixed << std::setprecision(7) 
                          << oldfitnessscore / 1000000.0 
                          << std::endl;

        std::cout << "\nCurrent Score: " << score << std::endl;

        // pressing enter proceeds to the next round
        std::cout << "\n\nPress Enter to continue to the next round...\n";
        std::cin.ignore();

        clearTerminal();

        // update startBoard for the next round
        startBoard = bestMoveSet.getFinalMoveSet();
    }

    std::cout << "\nGame Over! Final Score: " << score << " rounds played.\n";

    return 0;
}
