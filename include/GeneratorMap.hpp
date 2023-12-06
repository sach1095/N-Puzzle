#ifndef GENERATORMAP_HPP
# define GENERATORMAP_HPP

#include <vector>
#include <algorithm>
#include <random>

std::vector<int> makeGoal(int puzzleSize);
std::vector<int> makePuzzle(int puzzleSize, bool isSolvable);

#endif
