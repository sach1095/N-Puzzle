#include "Custom_Error.hpp"
#include "Parse.hpp"
#include "SearchAlgo.hpp"


// TODO Handle different heuristic function + different algorithm used
//! For now only using Manathan-distance as heuristic with A* algo

int main(int ac, char **av)
{
	try
	{
		if (ac == 1)
			throw CustomError("Error: no argments given.");
		Parse parser(av);

		// Init Puzzle size
		Puzzle::InitSizeLine(parser.getSizeline());

		auto algo = SearchAlgo(Algorithm::ASTAR, manhattan_distance, parser.getParsedContent());
		algo.Solve();
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
