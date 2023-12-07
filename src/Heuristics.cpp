
#include <cmath>

#include "Heuristics.hpp"
#include <iostream>

std::tuple<size_t, size_t> get_2dPosition(size_t position, size_t sizeLine)
{
	return {position / sizeLine, position % sizeLine};
}

// https://en.wikipedia.org/wiki/Taxicab_geometry
double manhattan_distance(const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<int>&vecSolution, const std::vector<size_t>& mapSolution)
{
	double distance  = 0;
	int x_pos,y_pos, x_sol, y_sol;
	(void)vecSolution;

	for (size_t i = 0; i < puzzleNumbers.size(); ++i)
	{
		// Get current position
		x_pos = i / sizeLine;
		y_pos = i % sizeLine;
		// Get the solution position for the number
		x_sol = mapSolution.at(puzzleNumbers[i]) / sizeLine;
		y_sol = mapSolution.at(puzzleNumbers[i]) % sizeLine;

		distance += std::abs(x_sol - x_pos) + std::abs(y_sol - y_pos);
	}

	return distance;
}

// Euclidean_heuristic
double euclidean_heuristic(const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<int>&vecSolution, const std::vector<size_t>& mapSolution)
{
	double distance  = 0;
	int x_pos,y_pos, x_sol, y_sol;
	(void)vecSolution;

	for (double i = 0; i < puzzleNumbers.size(); ++i)
	{
		// Get current position
		std::tie(x_pos, y_pos) = get_2dPosition(i, sizeLine);
		// Get the solution position for the number
		std::tie(x_sol, y_sol) = get_2dPosition(mapSolution.at(puzzleNumbers[i]), sizeLine);

		distance += sqrt(pow(x_sol - x_pos, 2.) + pow(y_sol - y_pos, 2.));
	}

	return distance;
}

// Count the number of tiles that are out of place.
double tiles_out_of_place(const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<int>&vecSolution, const std::vector<size_t>& mapSolution){
	(void)sizeLine;
	(void)mapSolution;
	size_t count = 0;

	for (size_t i = 0; i < puzzleNumbers.size(); ++i)
	{
		if (puzzleNumbers[i] != 0 && puzzleNumbers.at(i) != vecSolution.at(i))
			count++;
	}

	return count;
}
