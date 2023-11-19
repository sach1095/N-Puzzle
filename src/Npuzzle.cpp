#include "Custom_Error.hpp"
#include "Parse.hpp"
#include "SearchAlgo.hpp"


// TODO Handle different heuristic function + different algorithm used
//! For now only using Manathan-distance as heuristic with A* algo
//TODO Handle random generated map usage

int main(int ac, char **av)
{
	try
	{
		if (ac == 1)
			throw CustomError("Error: no argments given.");
		ParseFile parser(av);
		const std::vector<int>& parsedContent = parser.getParsedContent();
		if (parsedContent.size() > 10)
			throw CustomError("For now size > 10 is not handled");
		auto algo = SearchAlgo(Algorithm::ASTAR, manhattan_distance, parsedContent, parser.getSizeline());
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
