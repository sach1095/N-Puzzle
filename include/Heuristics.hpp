
#ifndef HEURISTICS_HPP
#define HEURISTICS_HPP

#include <unordered_map>
#include <tuple>
#include <vector>

typedef size_t (*heuristic)(const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<int>&vecSolution, const std::vector<size_t>& mapSolution);

size_t manhattan_distance( const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<int>&vecSolution, const std::vector<size_t>& mapSolution);
size_t euclidean_heuristic(const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<int>&vecSolution, const std::vector<size_t>& mapSolution);
size_t tiles_out_of_place( const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<int>&vecSolution, const std::vector<size_t>& mapSolution);

#endif
