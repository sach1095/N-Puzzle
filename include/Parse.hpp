#ifndef PARSFILE_HPP
#define PARSFILE_HPP

#include <queue>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <random>
#include <iomanip>
#include <cstring>
#include "Enum.hpp"
#include "Heuristics.hpp"

class Parse {
private:
	size_t							_sizeLine;
	double							_weight = 1.;
	Algorithm						_algo = ASTAR;
	heuristic						_heuristics = manhattan_distance;
	bool							_isSolvable = true;
	bool							_viewer = false;
	bool							_isFiles = false;
	std::queue<std::string>			_content;
	std::string						_fileName;
	std::vector<int>				_parsedContent;

public:

	Parse(int ac, char **av);
	const std::vector<int>& getParsedContent() const;
	void ParseArguments(char **av);
	void readInputFile();
	void parseContent();
	void showParsedContent();
	void verifyPuzzle();
	size_t getSizeline();
	double getWeight();
	Algorithm getAlgoSelected();
	void setHeuristicFunction(heuristic func);
	heuristic getHeuristicFunction();
	bool getviewer();
};

#endif
