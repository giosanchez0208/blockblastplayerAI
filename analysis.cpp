#include "move.h"
#include "board_state.h"
#include "my_blocks.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <fstream>

/*
g++ -g -o analysis analysis.cpp my_blocks.cpp move.cpp board_state.cpp block.cpp -std=c++11
./analysis
*/

struct GameAnalysisResult {
    int score;
    double annealingTime;
    double finalFitnessScore;
    
    GameAnalysisResult() : score(0), annealingTime(0.0), finalFitnessScore(0.0) {}
};

class GameAnalyzer {
private:
    int numGames;
    std::vector<GameAnalysisResult> gameResults;

public:
    GameAnalyzer(int games = 10) : numGames(games) {}

    void runAnalysis() {
        gameResults.clear();
        gameResults.reserve(numGames);

        for (int i = 0; i < numGames; ++i) {
            GameAnalysisResult result = playGame();
            gameResults.push_back(result);

            std::cout << "Completed " << (i + 1) << " games..." << std::endl;
        }
    }

    GameAnalysisResult playGame() {
        MyBlocks myBlocks = MyBlocks();
        BoardState startBoard = BoardState(0);
        GameAnalysisResult result;

        while (true) {
            Block block1 = myBlocks.getRandomBlock();
            Block block2 = myBlocks.getRandomBlock();
            Block block3 = myBlocks.getRandomBlock();

            auto startTime = std::chrono::high_resolution_clock::now();

            MoveSet bestMoveSet(startBoard, block1, block2, block3);

            try {
                bestMoveSet.anneal();
            } catch (const std::runtime_error& e) {
                break;
            }

            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = endTime - startTime;

            if (!bestMoveSet.canPerformMoveSet()) {
                break;
            }

            result.score++;
            result.annealingTime += duration.count();
            result.finalFitnessScore = bestMoveSet.getFinalMoveSet().getBoardFitness() / 1000000.0;

            startBoard = bestMoveSet.getFinalMoveSet();
        }

        return result;
    }

    void printResults() {
        if (gameResults.empty()) {
            std::cout << "No games have been analyzed yet." << std::endl;
            return;
        }

        // Calculate statistics
        std::vector<int> scores;
        std::vector<double> annealingTimes;
        std::vector<double> fitnessSscores;

        for (const auto& result : gameResults) {
            scores.push_back(result.score);
            annealingTimes.push_back(result.annealingTime);
            fitnessSscores.push_back(result.finalFitnessScore);
        }

        // Sort for percentile calculations
        std::sort(scores.begin(), scores.end());
        std::sort(annealingTimes.begin(), annealingTimes.end());
        std::sort(fitnessSscores.begin(), fitnessSscores.end());

        // Calculate metrics
        double avgScore = std::accumulate(scores.begin(), scores.end(), 0.0) / scores.size();
        double avgAnnealingTime = std::accumulate(annealingTimes.begin(), annealingTimes.end(), 0.0) / annealingTimes.size();
        double avgFitnessScore = std::accumulate(fitnessSscores.begin(), fitnessSscores.end(), 0.0) / fitnessSscores.size();

        // Percentiles
        size_t medianIndex = scores.size() / 2;
        int medianScore = scores[medianIndex];
        double p25Score = scores[scores.size() * 0.25];
        double p75Score = scores[scores.size() * 0.75];

        // Print results
        std::cout << "\n--- Game Analysis Results ---" << std::endl;
        std::cout << "Total Games Analyzed: " << numGames << std::endl;
        std::cout << "\nScore Metrics:" << std::endl;
        std::cout << "Average Score: " << std::fixed << std::setprecision(2) << avgScore << std::endl;
        std::cout << "Median Score: " << medianScore << std::endl;
        std::cout << "25th Percentile Score: " << p25Score << std::endl;
        std::cout << "75th Percentile Score: " << p75Score << std::endl;
        std::cout << "Minimum Score: " << scores.front() << std::endl;
        std::cout << "Maximum Score: " << scores.back() << std::endl;

        std::cout << "\nAnnealing Time Metrics:" << std::endl;
        std::cout << "Average Annealing Time: " 
                  << std::fixed << std::setprecision(7) 
                  << avgAnnealingTime << " seconds" << std::endl;

        std::cout << "\nFitness Score Metrics:" << std::endl;
        std::cout << "Average Final Fitness Score: " 
                  << std::fixed << std::setprecision(7) 
                  << avgFitnessScore << std::endl;
    }

    void exportResultsToCSV(const std::string& filename) {
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Unable to open file for writing: " << filename << std::endl;
            return;
        }

        // CSV Header
        outFile << "Game,Score,Annealing Time,Fitness Score\n";

        for (size_t i = 0; i < gameResults.size(); ++i) {
            const auto& result = gameResults[i];
            outFile << i + 1 << ","
                    << result.score << ","
                    << result.annealingTime << ","
                    << result.finalFitnessScore << "\n";
        }

        outFile.close();
        std::cout << "Results exported to " << filename << std::endl;
    }
};

int main() {
    GameAnalyzer analyzer(1000);
    
    std::cout << "Starting game analysis..." << std::endl;
    
    analyzer.runAnalysis();
    analyzer.printResults();

    analyzer.exportResultsToCSV("block_blast_analysis_results.csv");

    return 0;
}