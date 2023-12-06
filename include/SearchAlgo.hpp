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

namespace std
{
    template <>
    struct hash<VecWithHash>
	{
		std::size_t operator()(const VecWithHash& puzzle) const noexcept
		{
			return puzzle.Hash;
		}
	};
}

using setType = std::unordered_map<VecWithHash, PuzzleExtraInfo>;
using setIterator = setType::iterator;


namespace std
{
    template <>
    struct hash<setIterator>
	{
        size_t operator()(const setIterator& key) const {
            return key->first.Hash;
        }
	};
	template <>
    struct greater<setIterator>
	{
        bool operator()(const setIterator& lhs, const setIterator& rhs) const
		{
			return lhs->second.GetTotalCost() < rhs->second.GetTotalCost();
        }
    };
	template <>
    struct less<setIterator>
	{
        bool operator()(const setIterator& lhs, const setIterator& rhs) const
		{
			return lhs->second.GetTotalCost() > rhs->second.GetTotalCost();
        }
    };
}

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
    std::priority_queue<setIterator, std::vector<setIterator>> OpenedSet;
    setType		 ClosedSet;
	const std::vector<int> &Solution;
};

#endif
