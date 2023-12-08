#include "Custom_Error.hpp"
#include "Parse.hpp"
#include "SearchAlgo.hpp"
#include "ViewerSolution.hpp"
#include "isNotSolvable.hpp"

int main(int ac, char **av)
{
	try
	{
		Parse parser(ac, av);

		size_t sizePuzzle = parser.getSizeline();

		// Init Puzzle size
		PuzzleExtraInfo::InitSizeLine(sizePuzzle);

		// Verification of the solvability of the map
		if (isNotSolvable(parser.getParsedContent(), sizePuzzle))
			throw CustomError("Error: this map is not solvable.");

		auto algo = SearchAlgo(parser.getAlgoSelected(), parser.getHeuristicFunction(), parser.getWeight(), parser.getParsedContent());
		if (algo.Solve() && parser.getviewer())
			reconstructAndDisplayPath(parser.getParsedContent(), sizePuzzle, algo.getReverseMoveSolution());
	}
	catch(const CustomError& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const std::exception& e) {
		std::cerr << "Std Error: " << e.what() << std::endl;
	}
	return 0;
}
