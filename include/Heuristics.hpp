
#ifndef HEURISTICS_HPP
#define HEURISTICS_HPP

#include <unordered_map>
#include <tuple>
#include <vector>

typedef double (*heuristic)(const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<int>&vecSolution, const std::vector<size_t>& mapSolution);

double manhattan_distance( const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<int>&vecSolution, const std::vector<size_t>& mapSolution);
double euclidean_heuristic(const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<int>&vecSolution, const std::vector<size_t>& mapSolution);
double tiles_out_of_place( const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<int>&vecSolution, const std::vector<size_t>& mapSolution);

#endif
