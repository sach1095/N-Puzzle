
#include "SearchAlgo.hpp"

SearchAlgo::SearchAlgo(Algorithm algo_used, heuristic heuristic_used,
                       std::vector<int> puzzleNumbers, size_t size_line)
                : AlgorithmUsed(algo_used), HeuristicFunction(heuristic_used)
{
    // Init Puzzle size
    Puzzle::InitSizeLine(size_line);
    this->InitPuzzlePtr = std::make_unique<Puzzle>(puzzleNumbers, 0, this->HeuristicFunction(puzzleNumbers, Puzzle::GetSizeLine()), nullptr);
    std::cout << "Init puzzle : " << *this->InitPuzzlePtr << std::endl;
}
