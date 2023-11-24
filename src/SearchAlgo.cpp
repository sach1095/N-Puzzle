
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

std::vector<Puzzle> SearchAlgo::FindNeighbors(const Puzzle& currentPuzzle, size_t newPathCost)
{
	std::vector<Puzzle> neighbors;
	const size_t zeroPositon = currentPuzzle.GetPositionZero();

	// Left
	if (zeroPositon % Puzzle::GetSizeLine() != 0)
	{
		std::vector<int> leftMoveCopy = currentPuzzle.GetNumbers();
		std::swap(leftMoveCopy[zeroPositon], leftMoveCopy[zeroPositon - 1]);
		neighbors.emplace_back(leftMoveCopy, zeroPositon - 1, &currentPuzzle, LEFT, newPathCost, 0);
	}

	// Right
	if (zeroPositon % Puzzle::GetSizeLine() != Puzzle::GetSizeLine() - 1)
	{
		std::vector<int> leftMoveCopy = currentPuzzle.GetNumbers();
		std::swap(leftMoveCopy[zeroPositon], leftMoveCopy[zeroPositon + 1]);
		neighbors.emplace_back(leftMoveCopy, zeroPositon + 1, &currentPuzzle, RIGHT, newPathCost, 0);
	}

	// Up
	if (zeroPositon / Puzzle::GetSizeLine() != 0)
	{
		std::vector<int> leftMoveCopy = currentPuzzle.GetNumbers();
		std::swap(leftMoveCopy[zeroPositon], leftMoveCopy[zeroPositon - Puzzle::GetSizeLine()]);
		neighbors.emplace_back(leftMoveCopy, zeroPositon - Puzzle::GetSizeLine(), &currentPuzzle, UP, newPathCost, 0);
	}

	// Down
	if (zeroPositon / Puzzle::GetSizeLine() != Puzzle::GetSizeLine() - 1)
	{
		std::vector<int> leftMoveCopy = currentPuzzle.GetNumbers();
		std::swap(leftMoveCopy[zeroPositon], leftMoveCopy[zeroPositon + Puzzle::GetSizeLine()]);
		neighbors.emplace_back(leftMoveCopy, zeroPositon + Puzzle::GetSizeLine(),  &currentPuzzle, DOWN, newPathCost, 0);
	}

	return neighbors;
}

void SearchAlgo::PrintSolution(const Puzzle& solution, size_t nbrLoop, size_t maxSizeClosedSet)
{
	std::cout << "Find a solution" << std::endl;
	std::cout << "Total number of states selected : " << nbrLoop << std::endl;
	std::cout << "Max number of states in memory : " << maxSizeClosedSet << std::endl;
	const Puzzle *lastValue = &solution;
	std::vector<Move> reverseMoveSolution;
	while (lastValue->GetPreviousPuzzle())
	{
		reverseMoveSolution.push_back(lastValue->GetLastMove());
		lastValue = lastValue->GetPreviousPuzzle();
	}
	std::cout << "Numbers of moves to the solution : " << reverseMoveSolution.size() << std::endl;
	for (auto it = reverseMoveSolution.rbegin(); it != reverseMoveSolution.rend(); ++it)
	{
		if (*it == LEFT)
			std::cout << "Left ";
		else if (*it == RIGHT)
			std::cout << "Right ";
		else if (*it == UP)
			std::cout << "Up ";
		else if (*it == DOWN)
			std::cout << "Down ";
	}
	std::cout << std::endl;
}

void SearchAlgo::Solve()
{

	// Init algorithm
	auto [it, success] = this->ClosedSet.insert(this->InitPuzzlePtr);
	this->OpenedSet.push(it);

	// Counters
	size_t nbrLoop = 1;
	size_t maxSizeClosedSet = 1;

	while (!this->OpenedSet.empty())
	{
		auto& top = *this->OpenedSet.top();
		this->OpenedSet.pop();

		// Find a solution : print details
		if (top == this->Solution)
		{
			PrintSolution(top, nbrLoop, maxSizeClosedSet);
			return;
		}

		size_t pathCostUpdated = (this->AlgorithmUsed == GREEDY) ? 0 : top.GetPathCost() + 1;
		// Loop over neightbors
		for (auto& neighbor : SearchAlgo::FindNeighbors(top, pathCostUpdated))
		{
			auto foundClosedSet = this->ClosedSet.find(neighbor);

			// If not in the set, add it
			if (foundClosedSet == this->ClosedSet.end())
			{
				if (this->AlgorithmUsed != UNIFORM_COST)
				{
					// Compute missing heuristic
					size_t neighborHeuristic = this->HeuristicFunction(neighbor.GetNumbers(), Puzzle::GetSizeLine(), Puzzle::GetMapSolution());
					neighbor.SetHeuristicValue(neighborHeuristic);
					neighbor.SetTotalCost(neighborHeuristic + neighbor.GetPathCost());
				}
				// Insert element
				auto [it, success] = this->ClosedSet.insert(std::move(neighbor));
				this->OpenedSet.emplace(it);
			}
			// If a better path is found, update the set and put in in the OpenSet
			else if (this->AlgorithmUsed != GREEDY &&
					 neighbor.GetPathCost() < foundClosedSet->GetPathCost())
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

			// Update counters
			nbrLoop++;
			if (this->ClosedSet.size() > maxSizeClosedSet)
				maxSizeClosedSet = this->ClosedSet.size();
		}
	}
	std::cout << "Unsolvable puzzle" << std::endl;
	std::cout << "Total number of states selected : " << nbrLoop << std::endl;
	std::cout << "Max number of states in memory : " << maxSizeClosedSet << std::endl;
}