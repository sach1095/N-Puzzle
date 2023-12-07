#include <fstream>
#include <iostream>
#include "PuzzleExtraInfo.hpp"
#include "Custom_Error.hpp"

int applyMove(std::vector<int>& puzzle, int puzzleSize, Move move) {
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
	return puzzle[emptyIndex];
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
	std::ofstream htmlFile("solutionviewer.html");
	if (!htmlFile.is_open())
		throw CustomError("Error: Unable to open file for writing.");

	htmlFile << "<!DOCTYPE html>\n<html>\n<head>\n<title>Puzzle Solution</title>\n";
	htmlFile << "<style>\n";
	htmlFile << "table { border-collapse: collapse; margin-bottom: 20px; }\n";
	htmlFile << "td { width: 40px; height: 40px; text-align: center; border: 1px solid black; }\n";
	htmlFile << "</style>\n";
	htmlFile << "</head>\n<body style='background-color: bisque; display: flex; flex-direction: column; align-items: center; font-size: 3rem;'>\n";

	htmlFile << "<div style='margin: 1rem; font-family: cursive;'>Initial State</div>\n";
	htmlFile << "<table style='position: relative; width: 612px; height: 612px; margin: 0 auto 25px auto; border: 2px solid #333; background-color: #FFF; box-shadow: 0 0 15px #333;  -moz-box-shadow: 0 0 15px #333;  -webkit-box-shadow: 0 0 15px #333; border: #8b4000; border-style: ridge; border-width: 1.5rem; background-color: peru;'>\n";
	for (int i = 0; i < puzzleSize; ++i) {
		htmlFile << "<tr>\n";
		for (int j = 0; j < puzzleSize; ++j) {
			if (puzzle[i * puzzleSize + j] == 0)
				htmlFile << "<td style='background-color: aliceblue; color: transparent; width: 200px !important; height: 200px !important; text-align: center;    border: #f1e1d3; border-style: ridge; border-width: 0.65rem;'>" << puzzle[i * puzzleSize + j] << "</td>\n";
			else
				htmlFile << "<td style='background-color: floralwhite; width: 200px !important; height: 200px !important; text-align: center;    border: #f1e1d3; border-style: ridge; border-width: 0.65rem;'>" << puzzle[i * puzzleSize + j] << "</td>\n";
		}
		htmlFile << "</tr>\n";
	}
	htmlFile << "</table>\n";

	for (auto it = moves.rbegin(); it != moves.rend(); ++it) {
		Move move = *it;
		int numbermoved = applyMove(puzzle, puzzleSize, move);
		htmlFile << "<div style='margin: 1rem; font-family: cursive;'>Move: " << moveToString(move) << "</div>\n";
		htmlFile << "<table style='position: relative; width: 612px; height: 612px; margin: 0 auto 25px auto; border: 2px solid #333; background-color: #FFF; box-shadow: 0 0 15px #333;  -moz-box-shadow: 0 0 15px #333;  -webkit-box-shadow: 0 0 15px #333; border: #8b4000; border-style: ridge; border-width: 1.5rem; background-color: peru;'>\n";
		for (int i = 0; i < puzzleSize; ++i) {
			htmlFile << "<tr>\n";
			for (int j = 0; j < puzzleSize; ++j) {
				if (puzzle[i * puzzleSize + j] == 0)
					htmlFile << "<td style='background-color: burlywood; color: transparent; width: 200px !important; height: 200px !important; text-align: center;  border: #f1e1d3; border-style: ridge; border-width: 0.65rem;'>" << puzzle[i * puzzleSize + j] << "</td>\n";
				else if (puzzle[i * puzzleSize + j] == numbermoved)
					htmlFile << "<td style='background-color: darkgoldenrod; width: 200px !important; height: 200px !important; text-align: center;    border: #f1e1d3; border-style: ridge; border-width: 0.65rem;'>" << puzzle[i * puzzleSize + j] << "</td>\n";
				else
					htmlFile << "<td style='background-color: floralwhite; width: 200px !important; height: 200px !important; text-align: center;    border: #f1e1d3; border-style: ridge; border-width: 0.65rem;'>" << puzzle[i * puzzleSize + j] << "</td>\n";
				}
			htmlFile << "</tr>\n";
		}
		htmlFile << "</table>\n";
	}
	htmlFile << "</body>\n</html>";
	htmlFile.close();
	std::cout << "HTML solution file created." << std::endl;
}
