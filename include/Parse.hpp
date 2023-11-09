#ifndef PARSFILE_HPP
#define PARSFILE_HPP

#include <queue>
#include <vector>
#include <fstream>
#include <iostream>

class ParseFile {
private:
	std::queue<std::string> _content;
	std::vector<std::string> _parsedContent;

public:

	ParseFile(const std::string& fileName);
	void setContent(std::queue<std::string>&& content);
	const std::vector<std::string>& getParsedContent() const;
	const std::queue<std::string>& getContent() const;
	void readInputFile(const std::string& fileName);
};

#endif
