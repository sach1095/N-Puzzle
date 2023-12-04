#ifndef SEARCHALGO_HPP
#define SEARCHALGO_HPP

#include <algorithm>
#include <array>
#include <chrono>
#include <memory>
#include <queue>
#include <unordered_set>
#include <set>
#include <vector>

#include "Color.hpp"
#include "Enum.hpp"
#include "Heuristics.hpp"
#include "PuzzleExtraInfo.hpp"
#include "ZobristHash.hpp"

using namespace std::chrono;

struct VecWithHash
{
	std::vector<int> VecNumbers;
	int Hash;
	size_t ZeroPos;

	VecWithHash() = default;
	VecWithHash(const std::vector<int>& vecNumbers, int hash, size_t zeroPos)
		: VecNumbers(vecNumbers), Hash(hash), ZeroPos(zeroPos) {}
};

inline bool operator==(const VecWithHash& lhs, const VecWithHash& rhs)
{
	return lhs.VecNumbers == rhs.VecNumbers;
}

struct HashPuzzle
{
	std::size_t operator()(const VecWithHash& puzzle) const noexcept
	{
		return puzzle.Hash;
	}
};

using setType = std::unordered_map<VecWithHash, PuzzleExtraInfo, HashPuzzle>;
using setIterator = setType::iterator;

// Comparaison structure for puzzle
struct ComparePuzzleCost
 {
   bool operator()(setIterator& l, setIterator& r)
   {
       return l->second.GetTotalCost() > r->second.GetTotalCost();
   }
 };

class SearchAlgo
{
public:
	SearchAlgo() = delete;
	SearchAlgo(Algorithm algo_used, heuristic heuristic_used,
			   std::vector<int> puzzleNumbers);

    bool Solve();
	const std::vector<Move>& getReverseMoveSolution(){return this->reverseMoveSolution;};
	void setReverseMoveSolution(std::vector<Move> reverseMoveSolution) {this->reverseMoveSolution = reverseMoveSolution;};
	VecWithHash SwapPuzzle(const std::vector<int>& vecPuzzle, size_t zeroPos, size_t newZeroPos, size_t hash) const;
	std::array<VecWithHash, 4> FindNeighbors(const VecWithHash& currentPuzzle) const;
	void PrintSolution(const PuzzleExtraInfo& solution, size_t nbrLoop, size_t maxSizeClosedSet);

private:

	// Parameters
	Algorithm	AlgorithmUsed;
	heuristic	HeuristicFunction;
	std::vector<Move> reverseMoveSolution;
	ZobristHash	Hasher = ZobristHash(PuzzleExtraInfo::GetSizeLine() * PuzzleExtraInfo::GetSizeLine());

    // Intern members
    PuzzleExtraInfo		InitPuzzle;
	VecWithHash	InitVecNumbers;
    std::priority_queue<setIterator, std::vector<setIterator>, ComparePuzzleCost> OpenedSet;
    setType		 ClosedSet;
	const std::vector<int> &Solution;
};

#endif
