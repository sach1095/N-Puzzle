
#ifndef HEURISTICS_HPP
#define HEURISTICS_HPP

#include <tuple>
#include <vector>

typedef size_t (*heuristic)(const std::vector<int>& puzzleNumbers, size_t sizeLine);


size_t manhattan_distance(const std::vector<int>& puzzleNumbers, size_t sizeLine);

// Utils
std::tuple<size_t, size_t> get_2dPosition(int position, size_t sizeLine);

#endif