
#include <cmath>

#include "Heuristics.hpp"

std::tuple<size_t, size_t> get_2dPosition(size_t position, size_t sizeLine)
{
	return {position / sizeLine, position % sizeLine};
}

// https://en.wikipedia.org/wiki/Taxicab_geometry
size_t manhattan_distance(const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<size_t>& vecSolution)
{
	size_t distance  = 0;
	int x_pos,y_pos, x_sol, y_sol;

	for (size_t i = 0; i < puzzleNumbers.size(); ++i)
	{
		// Get current position
		x_pos = i / sizeLine;
		y_pos = i % sizeLine;
		// Get the solution position for the number
		x_sol = vecSolution.at(puzzleNumbers[i]) / sizeLine;
		y_sol = vecSolution.at(puzzleNumbers[i]) % sizeLine;

		distance += std::abs(x_sol - x_pos) + std::abs(y_sol - y_pos);
	}

	return distance;
}

// Linear conflicts occur when tiles are in their correct row or column but are reversed with another tile.
// Each linear conflict adds 2 to the cost (since at least two extra moves are necessary to resolve the conflict).
size_t linear_conflict(const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<size_t>& mapSolution) {
	size_t distance = manhattan_distance(puzzleNumbers, sizeLine, mapSolution);
	size_t conflict = 0;

	for (size_t i = 0; i < sizeLine; ++i) {
		for (size_t j = 0; j < sizeLine; ++j) {
			for (size_t k = j + 1; k < sizeLine; ++k) {
				// Check for linear conflicts on the line
				if (mapSolution.at(puzzleNumbers[i * sizeLine + j]) / sizeLine == i &&
					mapSolution.at(puzzleNumbers[i * sizeLine + k]) / sizeLine == i &&
					puzzleNumbers[i * sizeLine + j] > puzzleNumbers[i * sizeLine + k]) {
					conflict++;
				}
				// Check for linear conflicts on the column
				if (mapSolution.at(puzzleNumbers[j * sizeLine + i]) % sizeLine == i &&
					mapSolution.at(puzzleNumbers[k * sizeLine + i]) % sizeLine == i &&
					puzzleNumbers[j * sizeLine + i] > puzzleNumbers[k * sizeLine + i]) {
					conflict++;
				}
			}
		}
	}

	return distance + 2 * conflict;
}

// Count the number of tiles that are out of place.
size_t tiles_out_of_place(const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<size_t>& mapSolution) {
	(void)sizeLine;
	size_t count = 0;

	for (size_t i = 0; i < puzzleNumbers.size(); ++i) {
		if (puzzleNumbers[i] != 0 && mapSolution.at(puzzleNumbers[i]) != i) {
			count++;
		}
	}

	return count;
}
