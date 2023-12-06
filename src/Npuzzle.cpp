#include "Custom_Error.hpp"
#include "Parse.hpp"
#include "SearchAlgo.hpp"
#include "ViewerSolution.hpp"

int main(int ac, char **av)
{
	try
	{
		Parse parser(ac, av);

		// Init Puzzle size
		PuzzleExtraInfo::InitSizeLine(parser.getSizeline());
		heuristic heuristicToUse = parser.getHeuristicFunction();

		auto algo = SearchAlgo(parser.getAlgoSelected(), heuristicToUse, parser.getParsedContent());
		if (algo.Solve() && parser.getviewer())
			reconstructAndDisplayPath(parser.getParsedContent(), parser.getSizeline(), algo.getReverseMoveSolution());
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
