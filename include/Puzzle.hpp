#ifndef PUZZLE_HPP
#define PUZZLE_HPP

#include <iostream>
#include <tuple>
#include <vector>

#define SET_MACRO(name,type) void Set##name (type _arg) { name = _arg;}
#define GET_MACRO(name,type) type Get##name () const { return name;}

class Puzzle
{
public:
	Puzzle() = delete;
	Puzzle(const Puzzle& rhs) = default;
	Puzzle(const std::vector<int>& vecNumbers, size_t pathCost,
			size_t heuristic, Puzzle* previousPuzzle);

	void SetVisited() {this->HasBeenVisited = true;}
	bool operator==(const Puzzle& rhs) const;
	friend std::ostream& operator<<(std::ostream& os, const Puzzle& puzzle);

	// Getter / Setter
	static void InitSizeLine(size_t size);
	static size_t GetSizeLine() {return Puzzle::SizeLine;}
	GET_MACRO(Numbers, const std::vector<int>&);
	GET_MACRO(PreviousPuzzle, Puzzle*);
	GET_MACRO(HasBeenVisited, bool);
	GET_MACRO(PathCost, size_t);
	GET_MACRO(HeuristicValue, size_t);
	GET_MACRO(TotalCost, size_t);

	SET_MACRO(PathCost, size_t);
	SET_MACRO(TotalCost, size_t);
	SET_MACRO(PreviousPuzzle, Puzzle*);

private :
	static size_t			SizeLine;
	const std::vector<int>	Numbers;
	Puzzle* 				PreviousPuzzle;
	bool 					HasBeenVisited = false;

	// Costs
	size_t 					PathCost;
	const size_t 			HeuristicValue;
	size_t 					TotalCost;
};

// std::ostream& operator<<(std::ostream& os, const Puzzle& puzzle);

#endif
