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

class Puzzle
{
public:
    Puzzle() = default;
    Puzzle(const Puzzle& rhs) = default;
    Puzzle(const std::vector<int>& vecNumbers, size_t positionZero,
		   const Puzzle* previousPuzzle, Move lastMove, size_t pathCost, size_t heuristic);

    bool operator==(const Puzzle& rhs) const;
    friend std::ostream& operator<<(std::ostream& os, const Puzzle& puzzle);
    static const Puzzle& GetSolution();
	static const std::unordered_map<int, size_t> GetMapSolution();

    // Getter / Setter
    static size_t GetSizeLine();
    GET_MACRO(Numbers, const std::vector<int>&);
	GET_MACRO(PositionZero, size_t);
    GET_MACRO(PreviousPuzzle, const Puzzle*);
	GET_MACRO(LastMove, Move);
    GET_MACRO(PathCost, size_t);
    GET_MACRO(HeuristicValue, size_t);
    GET_MACRO(TotalCost, size_t);

    static void InitSizeLine(size_t size);
    SET_MACRO(PreviousPuzzle, const Puzzle*);
	SET_MACRO(LastMove, Move);
    SET_MACRO(PathCost, size_t);
	SET_MACRO(HeuristicValue, size_t);
    SET_MACRO(TotalCost, size_t);

private :
    static size_t			SizeLine;
    std::vector<int>		Numbers;
	size_t 					PositionZero;
    const Puzzle* 			PreviousPuzzle;
    Move					LastMove;

    // Costs
    size_t 					PathCost;
    size_t 					HeuristicValue;
    size_t 					TotalCost;
};

#endif
