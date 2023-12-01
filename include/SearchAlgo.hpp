#ifndef SEARCHALGO_HPP
#define SEARCHALGO_HPP

#include <algorithm>
#include <chrono>
#include <memory>
#include <queue>
#include <unordered_set>
#include <set>
#include <vector>

#include "Color.hpp"
#include "Enum.hpp"
#include "Heuristics.hpp"
#include "Puzzle.hpp"
#include "ZobristHash.hpp"

using namespace std::chrono;

// Hash function for Puzzle
struct HashPuzzle
{
	std::size_t operator()(const Puzzle& puzzle) const noexcept
	{
		return puzzle.GetHashValue();
	}
};

using setType = std::unordered_set<Puzzle, HashPuzzle>;
using setIterator = setType::iterator;

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

    bool Solve();
    std::vector<Puzzle> FindNeighbors(const Puzzle& currentPuzzle, size_t newPathCost) const;
	void PrintSolution(const Puzzle& solution, size_t nbrLoop, size_t maxSizeClosedSet);
	void setReverseMoveSolution(std::vector<Move> reverseMoveSolution) {this->reverseMoveSolution = reverseMoveSolution;};
	std::vector<Move> getReverseMoveSolution(){return reverseMoveSolution;};
	Puzzle SwapPuzzle(const Puzzle& puzzle, size_t newZeroPos, size_t newPathCost, Move move) const;

private:

	// Parameters
	Algorithm                   AlgorithmUsed;
	heuristic                   HeuristicFunction;
	std::vector<Move>           reverseMoveSolution;
	ZobristHash					Hasher = ZobristHash(Puzzle::GetSizeLine() * Puzzle::GetSizeLine());

    // Intern members
    Puzzle					     InitPuzzlePtr;
    std::priority_queue<setIterator, std::vector<setIterator>, ComparePuzzleCost> OpenedSet;
    // Use the hash function of Puzzle
    setType						ClosedSet;
    size_t 						MaxSizeClosedSet = 0;
    size_t 						MaxSizeOpenedSet = 0;
	const Puzzle				&Solution;
};

#endif
