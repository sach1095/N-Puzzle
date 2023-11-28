#ifndef SEARCHALGO_HPP
#define SEARCHALGO_HPP

#include <algorithm>
#include <chrono>
#include <memory>
#include <queue>
#include <unordered_set>
#include <vector>

#include "Enum.hpp"
#include "Heuristics.hpp"
#include "Puzzle.hpp"

using namespace std::chrono;

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

using setIterator = std::unordered_set<Puzzle>::iterator;

// Comparaison structure for puzzle
struct ComparePuzzleCost
 {
   bool operator()(setIterator& l, setIterator& r)
   {
       return l->GetTotalCost() > r->GetTotalCost();
   }
 };

class SearchAlgo
{
public:
	SearchAlgo() = delete;
	SearchAlgo(Algorithm algo_used, heuristic heuristic_used,
			   std::vector<int> puzzleNumbers);

    void Solve();
    static std::vector<Puzzle> FindNeighbors(const Puzzle& currentPuzzle, size_t newPathCost);
	static void PrintSolution(const Puzzle& solution, size_t nbrLoop, size_t maxSizeClosedSet);

private:

	// Parameters
	Algorithm                   AlgorithmUsed;
	heuristic                   HeuristicFunction;

    // Intern members
    Puzzle					     InitPuzzlePtr;
    std::priority_queue<setIterator, std::vector<setIterator>, ComparePuzzleCost> OpenedSet;
    // Use the hash function of Puzzle
    std::unordered_set<Puzzle>	ClosedSet;
    size_t 						MaxSizeClosedSet = 0;
    size_t 						MaxSizeOpenedSet = 0;
	const Puzzle				&Solution;
};

// Helper functions
std::vector<int> flatten(const std::vector<std::vector<int>>& vecOfVec);

#endif
