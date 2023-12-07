#include "GeneratorMap.hpp"

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
