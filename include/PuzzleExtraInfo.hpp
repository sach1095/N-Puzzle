#ifndef PUZZLE_HPP
#define PUZZLE_HPP

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>

#include "Enum.hpp"

#define SET_MACRO(name,type) void Set##name (type _arg) { name = _arg;}
#define GET_MACRO(name,type) type Get##name () const { return name;}

class PuzzleExtraInfo
{
public:
    PuzzleExtraInfo() = default;
    PuzzleExtraInfo(const PuzzleExtraInfo* previousPuzzle, Move lastMove, size_t pathCost, size_t heuristic);

    friend std::ostream& operator<<(std::ostream& os, const PuzzleExtraInfo& puzzle);
    static const std::vector<int>& GetSolution();
	static const std::vector<size_t>& GetVecSolution();

    // Getter / Setter
    static size_t GetSizeLine();
    GET_MACRO(PreviousPuzzle, const PuzzleExtraInfo*);
	GET_MACRO(LastMove, Move);
    GET_MACRO(PathCost, size_t);
    GET_MACRO(HeuristicValue, double);
    GET_MACRO(TotalCost, double);

    static void InitSizeLine(size_t size);
    SET_MACRO(PreviousPuzzle, const PuzzleExtraInfo*);
	SET_MACRO(LastMove, Move);
    SET_MACRO(PathCost, size_t);
	SET_MACRO(HeuristicValue, size_t);
    SET_MACRO(TotalCost, size_t);

private :
    static size_t			SizeLine;
    const PuzzleExtraInfo* 	PreviousPuzzle;
    Move					LastMove;
    // Costs
    size_t 					PathCost;
    double 					HeuristicValue;
    double 					TotalCost;
};

#endif
