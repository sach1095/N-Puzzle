
#ifndef HEURISTICS_HPP
#define HEURISTICS_HPP

#include <unordered_map>
#include <tuple>
#include <vector>

typedef size_t (*heuristic)(const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<size_t>& vecSolution);


size_t manhattan_distance(const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<size_t>& mapSolution);
size_t linear_conflict(const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<size_t>& mapSolution);
size_t tiles_out_of_place(const std::vector<int>& puzzleNumbers, size_t sizeLine, const std::vector<size_t>& mapSolution);

// Utils
std::tuple<size_t, size_t> get_2dPosition(size_t position, size_t sizeLine);

#endif
