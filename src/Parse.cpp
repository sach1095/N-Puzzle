#include "Parse.hpp"
#include "Custom_Error.hpp"
#include <filesystem>

ParseFile::ParseFile(char **av) {
	this->_isSolvable = true;
	ParseAV(av);
	std::cout << "Valid content afther parsing :" << std::endl;
	this->showParsedContent();
}

const std::vector<int>& ParseFile::getParsedContent() const {
	return this->_parsedContent;
}

size_t	ParseFile::getSizeline(){
	return this->_sizeLine;
}

void ParseFile::readInputFile() {
	const std::string& filePathStr = this->_fileName;
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

	if (this->_content.empty())
		throw CustomError("Error: the file " + filePathStr + " is empty.");

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

		for (const auto& token : tokens) {
			if (!token.empty() && token[0] == '#')
				break;
			else if (!token.empty() && std::all_of(token.begin(), token.end(), ::isdigit)){
				this->_parsedContent.push_back(std::stoi(token));
			}
			else if (!token.empty())
				throw CustomError("Error: unknown token '" + token + "' in the file.");
		}
		this->_content.pop();
	}
	if (this->_parsedContent.empty())
		throw CustomError("Error: the file parsed is empty.");

	this->_sizeLine = static_cast<size_t>(this->_parsedContent[0]);
	this->_parsedContent.erase(this->_parsedContent.begin());
}

void ParseFile::verifyPuzzle() {

	if (this->_sizeLine < 3) {
		throw CustomError("Error: Bad size map, minimum size is 3.");
	}

	size_t totalElements = this->_parsedContent.size();

	// Ensure the total number of elements is consistent with the expected size of each line
	if (totalElements != this->_sizeLine * this->_sizeLine) {
		throw CustomError("Error: the puzzle does not match the expected size.");
	}

	std::set<size_t> seenNumbers;
	for (size_t num : this->_parsedContent) {
		if (num >= totalElements) {
			throw CustomError("Error: number (" + std::to_string(num) + ") out of range found in the puzzle.");
		}
		seenNumbers.insert(num);
	}

	if (seenNumbers.size() != totalElements) {
		throw CustomError("Error: missing or duplicate numbers in the puzzle.");
	}
}

void ParseFile::showParsedContent() {

	for (size_t i = 0; i < this->_parsedContent.size(); ++i) {
		std::cout << this->_parsedContent[i] << ' ';
		if ((i + 1) % this->_sizeLine == 0) {
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}

std::vector<int> makeGoal(int s) {
	int ts = s * s;
	std::vector<int> puzzle(ts, -1);
	int cur = 1, x = 0, ix = 1, y = 0, iy = 0;
	while (true) {
		puzzle[x + y * s] = cur;
		if (cur == 0) break;
		cur++;
		if (x + ix == s || x + ix < 0 || (ix != 0 && puzzle[x + ix + y * s] != -1)) {
			iy = ix;
			ix = 0;
		} else if (y + iy == s || y + iy < 0 || (iy != 0 && puzzle[x + (y + iy) * s] != -1)) {
			ix = -iy;
			iy = 0;
		}
		x += ix;
		y += iy;
		if (cur == s * s) cur = 0;
	}
	return puzzle;
}

void swapEmpty(std::vector<int>& p, int s) {
	int idx = std::distance(p.begin(), std::find(p.begin(), p.end(), 0));
	std::vector<int> poss;
	if (idx % s > 0) poss.push_back(idx - 1);
	if (idx % s < s - 1) poss.push_back(idx + 1);
	if (idx / s > 0) poss.push_back(idx - s);
	if (idx / s < s - 1) poss.push_back(idx + s);

	std::random_device rd;
	std::mt19937 g(rd());
	int swi = poss[std::uniform_int_distribution<>(0, poss.size() - 1)(g)];

	std::swap(p[idx], p[swi]);
}

std::vector<int> makePuzzle(int size, bool isSolvable) {
	std::vector<int> p = makeGoal(size);
	for (int i = 0; i < 4096; ++i) {
		swapEmpty(p, size);
	}

	if (!isSolvable) {
		if (p[0] == 0 || p[1] == 0) {
			std::swap(p[p.size() - 1], p[p.size() - 2]);
		} else {
			std::swap(p[0], p[1]);
		}
	}

	return p;
}

void ParseFile::ParseAV(char **av){

	std::queue<std::string>	args;
	int	otherFlag = 0;
	for (size_t i = 1; av[i]; i++)
		args.push(av[i]);
	while (!args.empty())
	{
		if (args.front() == "--size")
		{
			otherFlag++;
			args.pop();
			if (args.empty())
				throw CustomError("Error: No given argument for --size");
			try
			{
				int temp = std::stoi(args.front());
				if (temp < 2)
					throw CustomError("");
				this->_sizeLine = static_cast<size_t>(temp);
				args.pop();
			}
			catch(const std::exception& e){
				throw CustomError("Error: Bad argument for --size, " + args.front() + ".");
			}
		}
		else if (args.front() == "--solvable"){
			args.pop();
			otherFlag++;
			if (args.empty())
				throw CustomError("Error: No given argument for --solvable");
			if (args.front() == "true")
				this->_isSolvable = true;
			else if (args.front() == "false")
				this->_isSolvable = false;
			else
				throw CustomError("Error: Bad argument for --solvable, is need to be true or false.");
			args.pop();
		}
		else if (args.front() == "--file"){
			args.pop();
			if (otherFlag != 0)
				throw CustomError("Error: if --size or --solvable is set, using flag --file is not allowed.");
			if (args.empty())
				throw CustomError("Error: No given argument for --file");
			this->_fileName = args.front();
			this->_isFiles = true;
			args.pop();
		}
		else if (args.front() == "--help"){
			args.pop();
			std::cout << "Usage :" << std::endl;
			std::cout << "\n./N-Puzzle [--file] [path to file] \nor" << std::endl;
			std::cout << "./N-Puzzle [--size] [(number)size of the map] [--solvable (optional)] [(boolean) 'true'(default) or 'false']" << std::endl;
			exit(0);
		}
		else
			throw CustomError("Error: Unrecognized args : " + args.front());
	}

	if (!this->_isFiles){
		if (this->_sizeLine < 2)
			throw CustomError("Error: Bad argument for --size, the minimum for a map size is 3");
		this->_parsedContent = makePuzzle(this->_sizeLine, this->_isSolvable);
	}
	else{
		this->readInputFile();
		this->parseContent();
	}
	this->verifyPuzzle();
}
