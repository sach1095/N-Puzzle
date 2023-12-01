#include <fstream>
#include <iostream>
#include "../include/Puzzle.hpp"
#include "../include/Custom_Error.hpp"

void applyMove(std::vector<int>& puzzle, int puzzleSize, Move move) {
	int emptyIndex = std::find(puzzle.begin(), puzzle.end(), 0) - puzzle.begin();
	int swapIndex = 0;

	switch (move) {
		case LEFT:
			swapIndex = emptyIndex - 1;
			break;
		case RIGHT:
			swapIndex = emptyIndex + 1;
			break;
		case UP:
			swapIndex = emptyIndex - puzzleSize;
			break;
		case DOWN:
			swapIndex = emptyIndex + puzzleSize;
			break;
		default:
			break;
	}
	std::swap(puzzle[emptyIndex], puzzle[swapIndex]);
}

std::string moveToString(Move move) {
	switch (move) {
		case LEFT: return "Left";
		case RIGHT: return "Right";
		case UP: return "Up";
		case DOWN: return "Down";
		default: return "";
	}
}

void reconstructAndDisplayPath(std::vector<int> puzzle, int puzzleSize, const std::vector<Move>& moves) {
	std::ofstream htmlFile("solutionVisualiser.html");
	if (!htmlFile.is_open())
		throw CustomError("Error: Unable to open file for writing.");

	htmlFile << "<!DOCTYPE html>\n<html>\n<head>\n<title>Puzzle Solution</title>\n";
	htmlFile << "<style>\n";
	htmlFile << "table { border-collapse: collapse; margin-bottom: 20px; }\n";
	htmlFile << "td { width: 40px; height: 40px; text-align: center; border: 1px solid black; }\n";
	htmlFile << "</style>\n";
	htmlFile << "</head>\n<body>\n";

	htmlFile << "<div>Initial State</div>\n";
	htmlFile << "<table>\n";
	for (int i = 0; i < puzzleSize; ++i) {
		htmlFile << "<tr>\n";
		for (int j = 0; j < puzzleSize; ++j) {
			htmlFile << "<td>" << puzzle[i * puzzleSize + j] << "</td>\n";
		}
		htmlFile << "</tr>\n";
	}
	htmlFile << "</table>\n";

	for (auto it = moves.rbegin(); it != moves.rend(); ++it) {
		Move move = *it;
		applyMove(puzzle, puzzleSize, move);
		htmlFile << "<div>Move: " << moveToString(move) << "</div>\n";
		htmlFile << "<table>\n";
		for (int i = 0; i < puzzleSize; ++i) {
			htmlFile << "<tr>\n";
			for (int j = 0; j < puzzleSize; ++j) {
				htmlFile << "<td>" << puzzle[i * puzzleSize + j] << "</td>\n";
			}
			htmlFile << "</tr>\n";
		}
		htmlFile << "</table>\n";
	}
	htmlFile << "</body>\n</html>";
	htmlFile.close();
	std::cout << "HTML solution file created." << std::endl;
}
