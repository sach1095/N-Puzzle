
#include "SearchAlgo.hpp"

SearchAlgo::SearchAlgo(Algorithm algo_used, heuristic heuristic_used,
                       std::vector<std::vector<int>> initContent)
                : AlgorithmUsed(algo_used), HeuristicFunction(heuristic_used), Solution(Puzzle::GetSolution())
{
    auto puzzleNumbers = flatten(initContent);
	// Find zero
	size_t positionZero = std::distance(puzzleNumbers.begin(), std::find(puzzleNumbers.begin(), puzzleNumbers.end(), 0));

    this->InitPuzzlePtr = Puzzle(puzzleNumbers, positionZero, nullptr, NONE,
	0, this->HeuristicFunction(puzzleNumbers,Puzzle::GetSizeLine(), Puzzle::GetMapSolution()));
}

std::vector<int> flatten(const std::vector<std::vector<int>>& vecOfVec)
{
    std::vector<int> result;
    for(const auto &v: vecOfVec)
        result.insert(result.end(), v.begin(), v.end());
    return result;
}

std::vector<Puzzle> SearchAlgo::FindNeighbors(const Puzzle& currentPuzzle)
{
	std::vector<Puzzle> neighbors;
	const size_t zeroPositon = currentPuzzle.GetPositionZero();

	// Left
	if (zeroPositon % Puzzle::GetSizeLine() != 0)
	{
		std::vector<int> leftMoveCopy = currentPuzzle.GetNumbers();
		std::swap(leftMoveCopy[zeroPositon], leftMoveCopy[zeroPositon - 1]);
		neighbors.emplace_back(leftMoveCopy, zeroPositon - 1, &currentPuzzle, LEFT, currentPuzzle.GetPathCost() + 1, 0);
	}

	// Right
	if (zeroPositon % Puzzle::GetSizeLine() != Puzzle::GetSizeLine() - 1)
	{
		std::vector<int> leftMoveCopy = currentPuzzle.GetNumbers();
		std::swap(leftMoveCopy[zeroPositon], leftMoveCopy[zeroPositon + 1]);
		neighbors.emplace_back(leftMoveCopy, zeroPositon + 1, &currentPuzzle, RIGHT, currentPuzzle.GetPathCost() + 1, 0);
	}

	// Up
	if (zeroPositon / Puzzle::GetSizeLine() != 0)
	{
		std::vector<int> leftMoveCopy = currentPuzzle.GetNumbers();
		std::swap(leftMoveCopy[zeroPositon], leftMoveCopy[zeroPositon - Puzzle::GetSizeLine()]);
		neighbors.emplace_back(leftMoveCopy, zeroPositon - Puzzle::GetSizeLine(), &currentPuzzle, UP, currentPuzzle.GetPathCost() + 1, 0);
	}

	// Down
	if (zeroPositon / Puzzle::GetSizeLine() != Puzzle::GetSizeLine() - 1)
	{
		std::vector<int> leftMoveCopy = currentPuzzle.GetNumbers();
		std::swap(leftMoveCopy[zeroPositon], leftMoveCopy[zeroPositon + Puzzle::GetSizeLine()]);
		neighbors.emplace_back(leftMoveCopy, zeroPositon + Puzzle::GetSizeLine(),  &currentPuzzle, DOWN, currentPuzzle.GetPathCost() + 1, 0);
	}

	return neighbors;
}

bool SearchAlgo::Solve()
{
	auto [it, success] = this->ClosedSet.insert(this->InitPuzzlePtr);
	this->OpenedSet.push(it);

	while (!this->OpenedSet.empty())
	{
		auto& top = *this->OpenedSet.top();
		this->OpenedSet.pop();

		if (top == this->Solution)
		{
			std::cout << "Find a solution" << std::endl;
			const Puzzle *lastValue = &top;
			while (lastValue->GetPreviousPuzzle())
			{
				std::cout << lastValue->GetLastMove();
				lastValue = lastValue->GetPreviousPuzzle();
			}
			std::cout << std::endl;
			return true;
		}

		// Loop over neightbors
		for (auto& neighbor : SearchAlgo::FindNeighbors(top))
		{
			auto foundClosedSet = this->ClosedSet.find(neighbor);

			// If not in the set, add it
			if (foundClosedSet == this->ClosedSet.end())
			{
				// Compute missing heuristic
				size_t neighborHeuristic = this->HeuristicFunction(neighbor.GetNumbers(), Puzzle::GetSizeLine(), Puzzle::GetMapSolution());
				neighbor.SetHeuristicValue(neighborHeuristic);
				neighbor.SetTotalCost(neighborHeuristic + neighbor.GetPathCost());
				// Insert element
				auto [it, success] = this->ClosedSet.insert(std::move(neighbor));
				this->OpenedSet.emplace(it);
			}
			// If a better path is found, update the set and put in in the OpenSet
			else if (neighbor.GetPathCost() < foundClosedSet->GetPathCost())
			{
				auto& mutableSet = const_cast<Puzzle&>(*foundClosedSet);
				// Update previous puzzle
				mutableSet.SetPreviousPuzzle(&top);
				mutableSet.SetLastMove(neighbor.GetLastMove());
				// Update costs
				mutableSet.SetPathCost(neighbor.GetPathCost());
				mutableSet.SetTotalCost(mutableSet.GetPathCost() + mutableSet.GetHeuristicValue());
				// Add it back to the OpenSet
				this->OpenedSet.emplace(foundClosedSet);
			}
		}
	}
	std::cout << "No solution found" << std::endl;

	return false;
}