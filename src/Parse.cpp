#include "Parse.hpp"
#include "Custom_Error.hpp"
#include <filesystem>

Parse::Parse(char **inputArgs)
{
	this->_isSolvable = true;
	this->_fileName = false;
	this->_algo = ASTAR;
	this->_heuristics = MANHATTAN;
	ParseArguments(inputArgs);
	std::cout << "Valid content afther parsing :" << std::endl;
	this->showParsedContent();
}

const std::vector<int> &Parse::getParsedContent() const
{
	return this->_parsedContent;
}

Algorithm Parse::getAlgoSelected()
{
	return this->_algo;
};

Heuristics Parse::getHeuristicsSelected()
{
	return this->_heuristics;
};

size_t Parse::getSizeline()
{
	return this->_sizeLine;
}

void Parse::readInputFile()
{
	const std::string &filePathStr = this->_fileName;
	std::filesystem::path filePath(filePathStr);

	// Check if the path exists and is a file
	if (!std::filesystem::exists(filePath) || !std::filesystem::is_regular_file(filePath))
	{
		throw CustomError("Error: " + filePathStr + "doesn't correspond to any file");
	}

	std::string line;
	std::ifstream file(filePathStr);

	if (!file.is_open())
	{
		throw CustomError("Error: Can not open the file : " + filePathStr);
	}

	while (std::getline(file, line))
	{
		this->_content.push(std::move(line));
	}

	if (this->_content.empty())
		throw CustomError("Error: the file " + filePathStr + " is empty.");

	file.close();
}

std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;

	while (getline(ss, item, delim))
	{
		result.push_back(item);
	}

	return result;
}

void Parse::parseContent()
{
	while (!this->_content.empty())
	{
		const auto &line = this->_content.front();
		std::vector<std::string> tokens = split(line, ' ');

		for (const auto &token : tokens)
		{
			if (!token.empty() && token[0] == '#')
				break;
			else if (!token.empty() && std::all_of(token.begin(), token.end(), ::isdigit))
			{
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

void Parse::verifyPuzzle()
{

	if (this->_sizeLine < 3)
	{
		throw CustomError("Error: Bad size map, minimum size is 3.");
	}

	size_t totalElements = this->_parsedContent.size();

	// Ensure the total number of elements is consistent with the expected size of each row.
	if (totalElements != this->_sizeLine * this->_sizeLine)
	{
		throw CustomError("Error: the puzzle does not match the expected size.");
	}

	std::set<size_t> seenNumbers;
	for (size_t num : this->_parsedContent)
	{
		if (num >= totalElements)
		{
			throw CustomError("Error: number (" + std::to_string(num) + ") out of range found in the puzzle.");
		}
		seenNumbers.insert(num);
	}

	if (seenNumbers.size() != totalElements)
	{
		throw CustomError("Error: missing or duplicate numbers in the puzzle.");
	}
}

void Parse::showParsedContent()
{

	for (size_t i = 0; i < this->_parsedContent.size(); ++i)
	{
		std::cout << this->_parsedContent[i] << ' ';
		if ((i + 1) % this->_sizeLine == 0)
		{
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}

// Swaps the empty tile (0) with one of its adjacent tiles in the puzzle.
// This function is used for randomizing/shuffling the puzzle.
void swapEmptyTile(std::vector<int> &puzzle, int puzzleSize) {
	int emptyTileIndex = std::distance(puzzle.begin(), std::find(puzzle.begin(), puzzle.end(), 0));
	std::vector<int> possibleSwaps;

	// Add possible swap positions (left, right, above, below) to the list if they are within the puzzle bounds.
	if (emptyTileIndex % puzzleSize > 0)
		possibleSwaps.push_back(emptyTileIndex - 1); // Left
	if (emptyTileIndex % puzzleSize < puzzleSize - 1)
		possibleSwaps.push_back(emptyTileIndex + 1); // Right
	if (emptyTileIndex / puzzleSize > 0)
		possibleSwaps.push_back(emptyTileIndex - puzzleSize); // Above
	if (emptyTileIndex / puzzleSize < puzzleSize - 1)
		possibleSwaps.push_back(emptyTileIndex + puzzleSize); // Below

	// Randomly select one of the possible positions to swap with the empty tile.
	std::random_device rd;
	std::mt19937 generator(rd());
	int swapIndex = possibleSwaps[std::uniform_int_distribution<>(0, possibleSwaps.size() - 1)(generator)];

	// Perform the swap.
	std::swap(puzzle[emptyTileIndex], puzzle[swapIndex]);
}

// Generates a goal state for the puzzle in a spiral pattern.
std::vector<int> makeGoal(int puzzleSize) {
	int totalTiles = puzzleSize * puzzleSize;
	std::vector<int> puzzle(totalTiles, 0);
	int currentValue = 1, xCoord = 0, xStep = 1, yCoord = 0, yStep = 0;

	while (currentValue < totalTiles) {
		puzzle[xCoord + yCoord * puzzleSize] = currentValue;
		currentValue++;

		if (xCoord + xStep == puzzleSize || xCoord + xStep < 0 || (xStep != 0 && puzzle[xCoord + xStep + yCoord * puzzleSize] != 0)) {
			yStep = xStep;
			xStep = 0;
		} else if (yCoord + yStep == puzzleSize || yCoord + yStep < 0 || (yStep != 0 && puzzle[xCoord + (yCoord + yStep) * puzzleSize] != 0)) {
			xStep = -yStep;
			yStep = 0;
		}

		xCoord += xStep;
		yCoord += yStep;
	}

	return puzzle;
}

// Generates a puzzle of the specified size. The puzzle can be solvable or unsolvable based on the isSolvable parameter.
// The puzzle is initially set to the goal state and then shuffled to create the start state.
std::vector<int> makePuzzle(int puzzleSize, bool isSolvable) {
	std::vector<int> puzzle = makeGoal(puzzleSize); // Start with a solved puzzle.

	// Shuffle the puzzle by swapping the empty tile with adjacent tiles.
	for (int i = 0; i < 4096; ++i) {
		swapEmptyTile(puzzle, puzzleSize);
	}

	// If the puzzle should be unsolvable, swap the first two tiles (if possible) to make it unsolvable.
	if (!isSolvable) {
		if (puzzle[0] == 0 || puzzle[1] == 0) {
			std::swap(puzzle[puzzle.size() - 1], puzzle[puzzle.size() - 2]);
		} else {
			std::swap(puzzle[0], puzzle[1]);
		}
	}

	return puzzle;
}

void process_help()
{
	std::cout << "Usage :" << std::endl;
	std::cout << "\n./N-Puzzle [--file] [path to file]" << std::endl;
	std::cout << "or" << std::endl;

	std::cout << "./N-Puzzle [--size] [(number) size of the map] [--solvable (optional)] ['true' (default) or 'false']" << std::endl;

	std::cout << "\nAdditional optional flags:" << std::endl;
	std::cout << "--algo [astar/uniform/greedy]" << std::endl;

	std::cout << " - Astar: Uses a heuristic to find the shortest path to the solution." << std::endl;
	std::cout << " - Uniform: Explores paths based on their cumulative cost without using a heuristic." << std::endl;
	std::cout << " - Greedy: Uses only the heuristic to guide the search, disregarding past cost.\n" << std::endl;

	std::cout << "--heuristic [manhattan/linear/tiles]" << std::endl;

	std::cout << " - Manhattan: Calculates the cost based on the Manhattan distance of each tile to its target position." << std::endl;
	std::cout << " - Linear: Adds a penalty for linear conflicts to the Manhattan distance." << std::endl;
	std::cout << " - Tiles: Counts the number of tiles that are not in their correct location." << std::endl;

	exit(0);
}

void Parse::ParseArguments(char **inputArgs)
{
	std::queue<std::string> args;
	this->_isFiles = false;
	int otherFlag = 0;

	for (size_t i = 1; inputArgs[i]; i++)
	{
		if (strcmp(inputArgs[i], "--help") == 0)
			process_help();
		args.push(inputArgs[i]);
	}
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
				int resultCast = std::stoi(args.front());
				if (resultCast <= 2)
					throw CustomError("");
				this->_sizeLine = static_cast<size_t>(resultCast);
				args.pop();
			}
			catch (const std::exception &e)
			{
				throw CustomError("Error: Bad argument for --size, " + args.front() + ", the value needs to be a number and bigger than 2.");
			}
		}
		else if (args.front() == "--solvable")
		{
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
		else if (args.front() == "--file")
		{
			args.pop();
			if (args.empty())
				throw CustomError("Error: No given argument for --file");
			this->_fileName = args.front();
			this->_isFiles = true;
			args.pop();
		}
		else if (args.front() == "--algo")
		{
			args.pop();
			if (args.empty())
				throw CustomError("Error: No given argument for --algo");
			if (args.front() == "astar")
				this->_algo = ASTAR;
			else if (args.front() == "uniform")
				this->_algo = UNIFORM_COST;
			else if (args.front() == "greedy")
				this->_algo = GREEDY;
			else
				throw CustomError("Error: Unrecognized args for --algo, valide choose is [astar/uniform/greedy]");
			args.pop();
		}
		else if (args.front() == "--heuristic")
		{
			args.pop();
			if (args.empty())
				throw CustomError("Error: No given argument for --heuristic");
			if (args.front() == "manhattan")
				this->_heuristics = MANHATTAN;
			else if (args.front() == "linear")
				this->_heuristics = LINEAR_CONFLICT;
			else if (args.front() == "tiles")
				this->_heuristics = TILES;
			else
				throw CustomError("Error: Unrecognized args for --heuristic, valide choose is [manhattan/linear/tiles]");
			args.pop();
		}
		else
			throw CustomError("Error: Unrecognized args : " + args.front());
	}

	if (this->_isFiles)
	{
		if (otherFlag != 0)
			throw CustomError("Error: if --size or --solvable is set, using flag --file is not allowed.");
		this->readInputFile();
		this->parseContent();
	}
	else
	{
		if (this->_sizeLine < 2)
			throw CustomError("Error: Bad argument for --size, the minimum for a map size is 3");
		this->_parsedContent = makePuzzle(this->_sizeLine, this->_isSolvable);
	}
	this->verifyPuzzle();
}
