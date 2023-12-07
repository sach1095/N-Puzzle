#include "isNotSolvable.hpp"
#include "GeneratorMap.hpp"
#include "Heuristics.hpp"
#include <vector>
#include <iomanip>

bool isNotSolvable(std::vector<int> content, size_t sizeLine) {
	std::vector<int> solution = makeGoal(sizeLine);
	int sizeTotal = sizeLine * sizeLine - 1;
	int numberToSwap = 0;
	int zeroPosition = 0;
	int zeroPositionAfther = 0;
	int x_pos,y_pos, x_sol, y_sol;
	int countPermutations = 0;

	zeroPosition = std::distance(content.begin(), std::find(content.begin(), content.end(), 0));
	for (;0 < sizeTotal; sizeTotal--){
		numberToSwap = solution[sizeTotal];
		if (solution[sizeTotal] != content[sizeTotal]){
			auto indexNumberToSwap = std::distance(content.begin(), std::find(content.begin(), content.end(), numberToSwap));
			std::swap(content[indexNumberToSwap], content[sizeTotal]);
			countPermutations++;
		}
	}

	zeroPositionAfther = std::distance(content.begin(), std::find(content.begin(), content.end(), 0));

	x_pos = zeroPositionAfther / sizeLine;
	y_pos = zeroPositionAfther % sizeLine;
	x_sol = zeroPosition / sizeLine;
	y_sol = zeroPosition % sizeLine;

	auto distance = std::abs(x_sol - x_pos) + std::abs(y_sol - y_pos);
	return ((distance + countPermutations) % 2 != 0);
}
