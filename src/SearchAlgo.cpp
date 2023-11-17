
#include "SearchAlgo.hpp"

SearchAlgo::SearchAlgo(Algorithm algo_used, heuristic heuristic_used,
                       std::vector<std::vector<int>> initContent)
                : AlgorithmUsed(algo_used), HeuristicFunction(heuristic_used)
{
    // Init Puzzle size
    Puzzle::InitSizeLine(initContent.size());
    auto puzzleNumbers = flatten(initContent);
    this->InitPuzzlePtr = std::make_unique<Puzzle>(puzzleNumbers, 0, this->HeuristicFunction(puzzleNumbers, Puzzle::GetSizeLine()), nullptr);
    std::cout << "Init puzzle : " << *this->InitPuzzlePtr << std::endl;
}

std::vector<int> flatten(const std::vector<std::vector<int>>& vecOfVec)
{
    std::vector<int> result;
    for(const auto &v: vecOfVec)
        result.insert(result.end(), v.begin(), v.end());                                                                                         
    return result;
}
