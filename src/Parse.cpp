#include "Parse.hpp"
#include "Custom_Error.hpp"
#include <filesystem>

const std::vector<std::vector<int>>& ParseFile::getParsedContent() const {
	return this->_parsedContent;
}

void ParseFile::readInputFile(const std::string& filePathStr) {
	std::filesystem::path filePath(filePathStr);

	// Vérifier si le chemin existe et est un fichier
	if (!std::filesystem::exists(filePath) || !std::filesystem::is_regular_file(filePath)) {
		throw CustomError("Error: " + filePathStr + "doesn't correspond to any file");
	}

	std::string line;
	std::ifstream file(filePathStr);

	if (!file.is_open()) {
		throw CustomError("Error: Can not open the file : " + filePathStr);
	}

	while (std::getline(file, line)) {
		this->_content.push(std::move(line));
	}

	file.close();
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

void ParseFile::parseContent() {
    while (!this->_content.empty()) {
        const auto& line = this->_content.front();
        std::vector<std::string> tokens = split(line, ' ');

        std::vector<int> lineOfNumber;
        for (const auto& token : tokens) {
            if (!token.empty() && token[0] == '#')
                break;
            else if (!token.empty() && std::all_of(token.begin(), token.end(), ::isdigit))
                lineOfNumber.push_back(std::stoi(token));
            else if (!token.empty())
                throw CustomError("Error: unknown token '" + token + "' in the file.");
        }

        if (!lineOfNumber.empty()) {
            this->_parsedContent.push_back(lineOfNumber);
        }
        this->_content.pop();
    }
}

void ParseFile::checkWholeSequence() const {
    std::set<int> setNumberSeen;

    for (const auto& line : this->_parsedContent) {
        for (int num : line) {
            setNumberSeen.insert(num);
        }
    }

    size_t expectedSize = this->_parsedContent.size();
    size_t expectedTotalSize = expectedSize * expectedSize;

    if (setNumberSeen.size() != expectedTotalSize) {
        throw CustomError("Error: puzzle doesn't have the right number of unique value.");
    }

    for (size_t i = 0; i < expectedTotalSize; ++i) {
        if (setNumberSeen.find(static_cast<int>(i)) == setNumberSeen.end()) {
            throw CustomError("Error: puzzle number " + std::to_string(i) + " is missing.");
        }
    }
}

void ParseFile::checkSize() {
    if (this->_parsedContent.empty()) {
        throw CustomError("Error: The file is empty or does not have valid data.");
    }

    size_t expectedSize = static_cast<size_t>(this->_parsedContent[0][0]);
    if (this->_parsedContent.size() - 1 != expectedSize) {
        throw CustomError("Error: The number of line does not match the puzzle size.");
    }

    for (size_t i = 1; i < this->_parsedContent.size(); ++i) {
        if (this->_parsedContent[i].size() != expectedSize) {
            throw CustomError("Error: l" + std::to_string(i) + " puzzle is not a square.");
        }
    }
	this->_parsedContent.erase(this->_parsedContent.begin());
}

void ParseFile::showParsedContent() {
    for (const auto& line : this->_parsedContent) {
        for (int num : line) {
            std::cout << num << ' ';
        }
        std::cout << std::endl;
    }
}

ParseFile::ParseFile(const std::string& fileName) {
	this->readInputFile(fileName);
	this->parseContent();
	this->checkSize();
	this->checkWholeSequence();
	// std::cout << "Valid content afther parsing :" << std::endl;
	// this->showParsedContent();
}
