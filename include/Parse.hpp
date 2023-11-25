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

class Parse {
private:
	size_t							_sizeLine;
	bool							_isSolvable;
	bool							_isFiles;
	std::queue<std::string>			_content;
	std::string						_fileName;
	std::vector<int>				_parsedContent;

public:

	Parse(char **av);
	const std::vector<int>& getParsedContent() const;
	void ParseAV(char **av);
	void readInputFile();
	void parseContent();
	void showParsedContent();
	void verifyPuzzle();
	size_t getSizeline();
};

#endif
