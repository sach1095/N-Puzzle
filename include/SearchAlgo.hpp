#ifndef SEARCHALGO_HPP
#define SEARCHALGO_HPP

#include <memory>
#include <queue>
#include <unordered_set>
#include <vector>

#include "Enum.hpp"
#include "Heuristics.hpp"
#include "Puzzle.hpp"

// Hash function for Puzzle
template <>
struct std::hash<Puzzle>
{
    std::size_t operator()(const Puzzle& puzzle) const noexcept
    {
        size_t hash = 0;
        for (int nb : puzzle.GetNumbers())
            hash = nb ^ (hash << 1);
        return hash;
    }
};

class SearchAlgo
{
public:
    SearchAlgo() = delete;
    SearchAlgo(Algorithm algo_used, heuristic heuristic_used,
               std::vector<int> puzzleNumbers, size_t size_line);

    // TODO FUNCTIONS
    bool Solve();

private:

    // Parameters
    Algorithm                   AlgorithmUsed;
    heuristic                   HeuristicFunction;

    // Intern members
    std::unique_ptr<Puzzle>     InitPuzzlePtr;
    std::priority_queue<Puzzle> OpenedSet;
    // Use the hash function of Puzzle
    std::unordered_set<Puzzle>	ClosedSet;
    size_t 						MaxSizeClosedSet = 0;
    size_t 						MaxSizeOpenedSet = 0;
};


// TODO FUNCTIONS
std::vector<Puzzle> GetNeighbors(const Puzzle& puzzle);
bool UpdatePathCost(Puzzle& puzzle, size_t potentialNewPathCost, Puzzle* potentialParent);
bool IsSolution(const Puzzle& puzzle);

#endif
