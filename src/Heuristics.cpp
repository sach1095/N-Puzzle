
#include <cmath>

#include "Heuristics.hpp"

std::tuple<size_t, size_t> get_2dPosition(int position, size_t sizeLine)
{
	return {position /sizeLine, position % sizeLine};
}

// https://en.wikipedia.org/wiki/Taxicab_geometry
size_t manhattan_distance(const std::vector<int>& puzzleNumbers, size_t sizeLine)
{
	size_t distance  = 0;
	int x_pos,y_pos, x_sol, y_sol;

	for (size_t i = 0; i < puzzleNumbers.size(); ++i)
	{
		// Get current position
		std::tie(x_pos, y_pos) = get_2dPosition(static_cast<int>(i), sizeLine);
		// Get the solution position for the number
		std::tie(x_sol, y_sol) = get_2dPosition(puzzleNumbers[i], sizeLine);

		distance += std::abs(x_sol - x_pos) + std::abs(y_sol - y_pos);
	}

	return distance;
}