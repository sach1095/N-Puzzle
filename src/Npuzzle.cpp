#include "Npuzzle.hpp"
#include "Parse.hpp"

int main(int ac, char **av)
{
	try
	{
		//TODO Handle random generated map usage
		if (ac != 2)
			throw CustomError("Usage: ./Npuzzle [Path to map file]");
		ParseFile parser(av[1]);
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
