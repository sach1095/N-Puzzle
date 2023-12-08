
#include "PuzzleExtraInfo.hpp"
#include "Custom_Error.hpp"

// Initialize static var
size_t PuzzleExtraInfo::SizeLine = 0;

PuzzleExtraInfo::PuzzleExtraInfo(const PuzzleExtraInfo* previousPuzzle, Move lastMove, size_t pathCost, double heuristic, double totalCost) :
			   PreviousPuzzle(previousPuzzle), LastMove(lastMove),
			   PathCost(pathCost), HeuristicValue(heuristic), TotalCost(totalCost)
{
	if (!PuzzleExtraInfo::SizeLine)
		throw CustomError("Set Puzzle::SizeLine before using Puzzle class");
}

void PuzzleExtraInfo::InitSizeLine(size_t size)
{
	static bool alreadyInitialize = false;

	if (alreadyInitialize)
		throw CustomError("The size of the Puzzle has already been initialized");
	else
	{
		PuzzleExtraInfo::SizeLine = size;
		alreadyInitialize = true;
	}
}

size_t PuzzleExtraInfo::GetSizeLine()
{
	if (PuzzleExtraInfo::SizeLine == 0)
		throw CustomError("The size of the Puzzle has not been initialized");

	return PuzzleExtraInfo::SizeLine;
}

const std::vector<int>& PuzzleExtraInfo::GetSolution()
{
	size_t sizeLine = PuzzleExtraInfo::GetSizeLine();
	static std::vector<int> solution(sizeLine * sizeLine, 0);
	static bool firstTime = true;

	if (firstTime)
	{
		firstTime = false;
		int value = 1;
		int top = 0, bottom = sizeLine - 1, left = 0, right = sizeLine - 1;

		while (top <= bottom && left <= right) {
			// Move from left to right in the top row
			for (int i = left; i <= right; ++i)
				solution[top * sizeLine + i] = value++;
			top++;

			// Move from top to bottom in the rightmost column
			for (int i = top; i <= bottom; ++i)
				solution[i * sizeLine + right] = value++;
			right--;

			// Move from right to left in the bottom row
			if (top <= bottom) {
				for (int i = right; i >= left; --i)
					solution[bottom * sizeLine + i] = value++;
				bottom--;
			}

			// Move from bottom to top in the leftmost column
			if (left <= right) {
				for (int i = bottom; i >= top; --i)
					solution[i * sizeLine + left] = value++;
				left++;
			}
		}
		// Put the zero as the last value
		auto itZero = std::find(solution.begin(), solution.end(), value - 1);
		*itZero = 0;
	}

	return solution;
}

/**
 * Return a vector with the value of the puzzle piece as input and the position as output
*/
const std::vector<size_t>& PuzzleExtraInfo::GetVecSolution()
{
	static std::vector<size_t> vecSolution;

	if (vecSolution.empty())
	{
		vecSolution = std::vector<size_t>(PuzzleExtraInfo::GetSizeLine() * PuzzleExtraInfo::GetSizeLine());
		auto& solution = PuzzleExtraInfo::GetSolution();
		for (size_t i = 0; i < solution.size(); ++i)
			vecSolution[solution[i]] = i;
	}
	return vecSolution;
}


std::ostream& operator<<(std::ostream& os, const PuzzleExtraInfo& puzzle)
{
    os	<< "Puzzle(" << PuzzleExtraInfo::SizeLine << "): g = " << puzzle.PathCost
		<< " h = " << puzzle.HeuristicValue
		<< " total = " << puzzle.TotalCost << std::endl;

    os << std::endl;
    return os;
}
