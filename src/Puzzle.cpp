
#include "Puzzle.hpp"
#include "Custom_Error.hpp"

// Initialize static var
size_t Puzzle::SizeLine = 0;


Puzzle::Puzzle(const std::vector<int>& vecNumbers, size_t pathCost,
			   size_t heuristic, Puzzle* previousPuzzle) :
			   Numbers(vecNumbers), PreviousPuzzle(previousPuzzle),
			   PathCost(pathCost), HeuristicValue(heuristic), TotalCost(pathCost + heuristic)
{
	if (!Puzzle::SizeLine)
		throw CustomError("Set Puzzle::SizeLine before using Puzzle class");
}

bool Puzzle::operator==(const Puzzle& rhs) const
{
	return this->Numbers == rhs.Numbers;
}

void Puzzle::InitSizeLine(size_t size)
{
	static bool alreadyInitialize = false;

	if (alreadyInitialize)
		throw CustomError("The size of the Puzzle has already been initialized");
	else
	{
		Puzzle::SizeLine = size;
		alreadyInitialize = true;
	}
}


std::ostream& operator<<(std::ostream& os, const Puzzle& puzzle)
{
	os << "Puzzle of size : " << Puzzle::SizeLine << " and h value of : " << puzzle.GetHeuristicValue();
	auto& numbers = puzzle.GetNumbers();

	for (size_t i = 0; i < numbers.size(); ++i)
	{
		if (i % Puzzle::SizeLine == 0)
			os << std::endl;
		os << " " << numbers[i];
	}
	os << std::endl;
	return os;
}
