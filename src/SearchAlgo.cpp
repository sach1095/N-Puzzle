
#include "SearchAlgo.hpp"

SearchAlgo::SearchAlgo(Algorithm algo_used, heuristic heuristic_used,
											 std::vector<int> puzzleNumbers)
		: AlgorithmUsed(algo_used), HeuristicFunction(heuristic_used), Solution(Puzzle::GetSolution())
{
	// Find zero
	size_t positionZero = std::distance(puzzleNumbers.begin(), std::find(puzzleNumbers.begin(), puzzleNumbers.end(), 0));

	// Create the init puzzle
    this->InitPuzzlePtr = Puzzle(puzzleNumbers, positionZero, nullptr, NONE, 0,
								 this->HeuristicFunction(puzzleNumbers,Puzzle::GetSizeLine(), Puzzle::GetVecSolution()),
								 this->Hasher.Hash(puzzleNumbers));
}

std::vector<Puzzle> SearchAlgo::FindNeighbors(const Puzzle& currentPuzzle, size_t newPathCost) const
{
	std::vector<Puzzle> neighbors;
	const size_t zeroPositon = currentPuzzle.GetPositionZero();

	// Left
	if (zeroPositon % Puzzle::GetSizeLine() != 0)
	{
		std::vector<int> leftMoveCopy = currentPuzzle.GetNumbers();
		std::swap(leftMoveCopy[zeroPositon], leftMoveCopy[zeroPositon - 1]);

		// Compute new hash
		int i1 = zeroPositon;
		int j1 = leftMoveCopy[zeroPositon];
		int i2 = zeroPositon - 1;
		int j2 = leftMoveCopy[zeroPositon - 1];
		int newHash = this->Hasher.Swap(currentPuzzle.GetHashValue(), i1, j1, i2, j2);

		neighbors.emplace_back(leftMoveCopy, zeroPositon - 1, &currentPuzzle, LEFT, newPathCost, 0, newHash);
	}

	// Right
	if (zeroPositon % Puzzle::GetSizeLine() != Puzzle::GetSizeLine() - 1)
	{
		std::vector<int> leftMoveCopy = currentPuzzle.GetNumbers();
		std::swap(leftMoveCopy[zeroPositon], leftMoveCopy[zeroPositon + 1]);

		// Compute new hash
		int i1 = zeroPositon;
		int j1 = leftMoveCopy[zeroPositon];
		int i2 = zeroPositon + 1;
		int j2 = leftMoveCopy[zeroPositon + 1];
		int newHash = this->Hasher.Swap(currentPuzzle.GetHashValue(), i1, j1, i2, j2);

		neighbors.emplace_back(leftMoveCopy, zeroPositon + 1, &currentPuzzle, RIGHT, newPathCost, 0, newHash);
	}

	// Up
	if (zeroPositon / Puzzle::GetSizeLine() != 0)
	{
		std::vector<int> leftMoveCopy = currentPuzzle.GetNumbers();
		std::swap(leftMoveCopy[zeroPositon], leftMoveCopy[zeroPositon - Puzzle::GetSizeLine()]);

		// Compute new hash
		int i1 = zeroPositon;
		int j1 = leftMoveCopy[zeroPositon];
		int i2 = zeroPositon - Puzzle::GetSizeLine();
		int j2 = leftMoveCopy[zeroPositon - Puzzle::GetSizeLine()];
		int newHash = this->Hasher.Swap(currentPuzzle.GetHashValue(), i1, j1, i2, j2);

		neighbors.emplace_back(leftMoveCopy, zeroPositon - Puzzle::GetSizeLine(), &currentPuzzle, UP, newPathCost, 0, newHash);
	}

	// Down
	if (zeroPositon / Puzzle::GetSizeLine() != Puzzle::GetSizeLine() - 1)
	{
		std::vector<int> leftMoveCopy = currentPuzzle.GetNumbers();
		std::swap(leftMoveCopy[zeroPositon], leftMoveCopy[zeroPositon + Puzzle::GetSizeLine()]);

		// Compute new hash
		int i1 = zeroPositon;
		int j1 = leftMoveCopy[zeroPositon];
		int i2 = zeroPositon + Puzzle::GetSizeLine();
		int j2 = leftMoveCopy[zeroPositon + Puzzle::GetSizeLine()];
		int newHash = this->Hasher.Swap(currentPuzzle.GetHashValue(), i1, j1, i2, j2);

		neighbors.emplace_back(leftMoveCopy, zeroPositon + Puzzle::GetSizeLine(),  &currentPuzzle, DOWN, newPathCost, 0, newHash);
	}

	return neighbors;
}

void SearchAlgo::PrintSolution(const Puzzle &solution, size_t nbrLoop, size_t maxSizeClosedSet)
{
	// Find the list of positions to go to the solution
	const Puzzle *lastValue = &solution;
	std::vector<Move> reverseMoveSolution;
	while (lastValue->GetPreviousPuzzle())
	{
		reverseMoveSolution.push_back(lastValue->GetLastMove());
		lastValue = lastValue->GetPreviousPuzzle();
	}
	this->setReverseMoveSolution(reverseMoveSolution);

	// Print solution
	std::cout << BOLDGREEN 	 << "Find a solution" << RESET << std::endl;
	std::cout << "Total number of states selected  : "  << BOLDRED << nbrLoop 					<< RESET << std::endl;
	std::cout << "Max number of states in memory   : "   << BOLDRED << maxSizeClosedSet			<< RESET << std::endl;
	std::cout << "Numbers of moves to the solution : " << BOLDRED << reverseMoveSolution.size() << RESET << std::endl;
	std::cout << "List of moves : " << RESET << std::endl;
	std::cout << "[";
	for (auto it = reverseMoveSolution.rbegin(); it != reverseMoveSolution.rend(); ++it)
	{
		if (*it == LEFT)
			std::cout << BOLDYELLOW << "Left  " << RESET;
		else if (*it == RIGHT)
			std::cout << BOLDBLUE << "Right " << RESET;
		else if (*it == UP)
			std::cout << BOLDMAGENTA << " Up   " << RESET;
		else if (*it == DOWN)
			std::cout << BOLDBLACK << "Down  " << RESET;

		auto i = std::distance(reverseMoveSolution.rbegin(), it);
		if ((i + 1) % 10 == 0)
			std::cout << "\n ";
	}
	std::cout << "]" << std::endl;
}

// Delete the n last char in the console
void deleteConsoleNchar(size_t nchar)
{
	std::cout << std::string(nchar, '\b')
						<< std::string(nchar, ' ')
						<< std::string(nchar, '\b')
						<< std::flush;
}

// Print waiting message if needed or else erase it
inline void handleWaitingMessage(bool needPrint, size_t nbrLoop = 0,
								 size_t currentHeuristic = 0, size_t sizeOpenedSet = 0, size_t sizeClosedSet = 0)
{
	static std::string waitingMsg;
	static auto baseTime = steady_clock::now();
	static int count = 0;
	static auto startTime = steady_clock::now();

	if (!needPrint)
		deleteConsoleNchar(waitingMsg.size());
	else
	{
		if (waitingMsg.empty())
		{
			waitingMsg = "Computing";
			std::cout << waitingMsg << std::flush;
		}

		if (duration_cast<milliseconds>(steady_clock::now() - baseTime).count() > 1000)
		{
			baseTime = steady_clock::now();
			if (++count % 5 == 0)
			{
				deleteConsoleNchar(waitingMsg.size());
				auto duration = duration_cast<milliseconds>(baseTime - startTime);
				std::cout << GREEN << "****************** Temporary State (" << duration.count() / 1000.
								   << "s)******************" << RESET << std::endl;
				std::cout << "Number iterations    : " << BOLDRED << nbrLoop            << RESET << std::endl;
				std::cout << "Minimum heuristic    : " << BOLDRED << currentHeuristic   << RESET << std::endl;
				std::cout << "Size of the queue    : " << BOLDRED << sizeOpenedSet      << RESET << std::endl;
				std::cout << "Number puzzle loaded : " << BOLDRED << sizeClosedSet      << RESET << std::endl;
				std::cout << std::endl;
			}
			deleteConsoleNchar(waitingMsg.size());
			if (waitingMsg.size() == std::string("Computing...").size())
				waitingMsg = "Computing";
			else
				waitingMsg += '.';
			std::cout << waitingMsg << std::flush;
		}
	}
}

bool SearchAlgo::Solve()
{

	// Init algorithm
	auto [it, success] = this->ClosedSet.insert(this->InitPuzzlePtr);
	this->OpenedSet.push(it);

	// Counters
	size_t nbrLoop = 0;
	size_t sizeClosedSet = 0;

	while (!this->OpenedSet.empty())
	{
		nbrLoop++;
		auto &top = *this->OpenedSet.top();
		this->OpenedSet.pop();

		// Find a solution : print details
		if (top == this->Solution)
		{
			handleWaitingMessage(false);
			PrintSolution(top, nbrLoop, sizeClosedSet);
			return true;
		}

		// Stop the program if too much space is used
		if (this->OpenedSet.size() > 9e6 ||sizeClosedSet > 1.8e7)
		{
			handleWaitingMessage(false);
			std::cout << BOLDRED << "The program has exceeded memory limits. Exit" << std::endl;
			break;
		}

		// Output message than the program is computing
		handleWaitingMessage(true, nbrLoop, top.GetHeuristicValue(), this->OpenedSet.size(), sizeClosedSet);

		// Compute algorithm
		size_t pathCostUpdated = (this->AlgorithmUsed == GREEDY) ? 0 : top.GetPathCost() + 1;
		// Loop over neightbors
		for (auto& neighbor : this->FindNeighbors(top, pathCostUpdated))
		{
			auto foundClosedSet = this->ClosedSet.find(neighbor);

			// If not in the set, add it
			if (foundClosedSet == this->ClosedSet.end())
			{
				if (this->AlgorithmUsed != UNIFORM_COST)
				{
					// Compute missing heuristic
					size_t neighborHeuristic = this->HeuristicFunction(neighbor.GetNumbers(), Puzzle::GetSizeLine(), Puzzle::GetVecSolution());
					neighbor.SetHeuristicValue(neighborHeuristic);
					neighbor.SetTotalCost(neighborHeuristic + neighbor.GetPathCost());
				}
				// Insert element
				auto [it, success] = this->ClosedSet.insert(std::move(neighbor));
				this->OpenedSet.emplace(it);
				sizeClosedSet++;
			}
			// If a better path is found, update the set and put in in the OpenSet
			else if (this->AlgorithmUsed != GREEDY &&
							 neighbor.GetPathCost() < foundClosedSet->GetPathCost())
			{
				auto &mutableSet = const_cast<Puzzle &>(*foundClosedSet);
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

	handleWaitingMessage(false);
	std::cout << BOLDGREEN << "No solution found" << RESET << std::endl;
	std::cout << "Total number of states selected : " << BOLDRED  << nbrLoop       << RESET << std::endl;
	std::cout << "Max number of states in memory  : "  << BOLDRED  << sizeClosedSet << RESET << std::endl;
	return false;
}
