#ifndef PARSFILE_HPP
#define PARSFILE_HPP

#include <queue>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <set>

class ParseFile {
private:
	std::queue<std::string> _content;
	std::vector<std::vector<int>> _parsedContent;

public:

	ParseFile(const std::string& fileName);
	const std::vector<std::vector<int>>& getParsedContent() const;
	void readInputFile(const std::string& fileName);
	void parseContent();
	void showParsedContent();
	void checkWholeSequence() const;
	void checkSize();
};

#endif
