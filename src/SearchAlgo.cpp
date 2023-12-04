
#include "SearchAlgo.hpp"

SearchAlgo::SearchAlgo(Algorithm algo_used, heuristic heuristic_used,
                       std::vector<int> puzzleNumbers)
                     : AlgorithmUsed(algo_used), HeuristicFunction(heuristic_used), Solution(PuzzleExtraInfo::GetSolution())
{
	// Find zero
	size_t positionZero = std::distance(puzzleNumbers.begin(), std::find(puzzleNumbers.begin(), puzzleNumbers.end(), 0));

	// Create the init puzzle
    this->InitPuzzle = PuzzleExtraInfo(nullptr, NONE, 0, this->HeuristicFunction(puzzleNumbers,PuzzleExtraInfo::GetSizeLine(), PuzzleExtraInfo::GetVecSolution()));
	this->InitVecNumbers = {puzzleNumbers, this->Hasher.Hash(puzzleNumbers), positionZero};

	// Reserve memory
	std::vector<setIterator> openedSetMemory;
	if (PuzzleExtraInfo::GetSizeLine() == 3)
	{
		openedSetMemory.reserve(1e4);
		this->ClosedSet.reserve(1e4);
	}
	else if (PuzzleExtraInfo::GetSizeLine() == 4)
	{
		openedSetMemory.reserve(2e6);
		this->ClosedSet.reserve(2e6);
	}
	else if (PuzzleExtraInfo::GetSizeLine() == 5)
	{
		openedSetMemory.reserve(2e7);
		this->ClosedSet.reserve(2e7);
	}
	this->OpenedSet = std::priority_queue<setIterator, std::vector<setIterator>, ComparePuzzleCost>(ComparePuzzleCost(), std::move(openedSetMemory));
}

VecWithHash SearchAlgo::SwapPuzzle(const std::vector<int>& vecPuzzle, size_t zeroPos, size_t newZeroPos, size_t hash) const
{
	// Compute new hash
	int newHash = this->Hasher.Swap(hash, zeroPos, vecPuzzle[zeroPos], newZeroPos, vecPuzzle[newZeroPos]);

	// Create new vector
	std::vector<int> newVecPuzzle(vecPuzzle);
	std::swap(newVecPuzzle[zeroPos], newVecPuzzle[newZeroPos]);

	return {newVecPuzzle, newHash, newZeroPos};
}

std::array<VecWithHash, 4> SearchAlgo::FindNeighbors(const VecWithHash& currentPuzzle) const
{
	std::array<VecWithHash, 4> neighbors;
	size_t zeroPos = currentPuzzle.ZeroPos;

	// Left
	if (zeroPos % PuzzleExtraInfo::GetSizeLine() != 0)
		neighbors[0] = SwapPuzzle(currentPuzzle.VecNumbers, zeroPos, zeroPos - 1, currentPuzzle.Hash);

	// Right
	if (zeroPos % PuzzleExtraInfo::GetSizeLine() != PuzzleExtraInfo::GetSizeLine() - 1)
		neighbors[1] = SwapPuzzle(currentPuzzle.VecNumbers, zeroPos, zeroPos + 1, currentPuzzle.Hash);

	// Up
	if (zeroPos / PuzzleExtraInfo::GetSizeLine() != 0)
		neighbors[2] = SwapPuzzle(currentPuzzle.VecNumbers, zeroPos, zeroPos - PuzzleExtraInfo::GetSizeLine(), currentPuzzle.Hash);

	// Down
	if (zeroPos / PuzzleExtraInfo::GetSizeLine() != PuzzleExtraInfo::GetSizeLine() - 1)
		neighbors[3] = SwapPuzzle(currentPuzzle.VecNumbers, zeroPos, zeroPos + PuzzleExtraInfo::GetSizeLine(), currentPuzzle.Hash);

	return neighbors;
}

void SearchAlgo::PrintSolution(const PuzzleExtraInfo &solution, size_t nbrLoop, size_t maxSizeClosedSet)
{
	// Find the list of positions to go to the solution
	const PuzzleExtraInfo *lastValue = &solution;
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
	auto [it, success] = this->ClosedSet.insert({this->InitVecNumbers, this->InitPuzzle});
	this->OpenedSet.push(it);

	// Counters
	size_t nbrLoop = 0;
	size_t sizeClosedSet = 0;

	while (!this->OpenedSet.empty())
	{
		nbrLoop++;
		const auto& top = this->OpenedSet.top();
		const auto& keyPuzzle = top->first;
		const auto& valuePuzzle = top->second;
		this->OpenedSet.pop();

		// Find a solution : print details
		if (keyPuzzle.VecNumbers == this->Solution)
		{
			handleWaitingMessage(false);
			PrintSolution(valuePuzzle, nbrLoop, sizeClosedSet);
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
		handleWaitingMessage(true, nbrLoop, valuePuzzle.GetHeuristicValue(), this->OpenedSet.size(), sizeClosedSet);

		// Compute algorithm
		size_t pathCostUpdated = (this->AlgorithmUsed == GREEDY) ? 0 : valuePuzzle.GetPathCost() + 1;
		// Loop over neightbors
		auto vecNeighbors = this->FindNeighbors(keyPuzzle);
		for (int i = 0; i < 4; ++i)
		{
			const auto& neighbor = vecNeighbors[i];
			if (neighbor.VecNumbers.empty())
				continue;

			auto foundClosedSet = this->ClosedSet.find(neighbor);
			static const Move lastMove[4] = {LEFT, RIGHT, UP, DOWN};

			// If not in the set, add it
			if (foundClosedSet == this->ClosedSet.end())
			{
				size_t neighborHeuristic = (this->AlgorithmUsed == UNIFORM_COST) ?
											0 :
											this->HeuristicFunction(neighbor.VecNumbers, PuzzleExtraInfo::GetSizeLine(), PuzzleExtraInfo::GetVecSolution());
				// Insert element
				auto [it, success] = this->ClosedSet.insert({neighbor, PuzzleExtraInfo(&valuePuzzle, lastMove[i], pathCostUpdated, neighborHeuristic)});
				this->OpenedSet.emplace(it);
				sizeClosedSet++;
			}
			// If a better path is found, update the set and put in in the OpenSet
			else if (this->AlgorithmUsed != GREEDY &&
							 pathCostUpdated < foundClosedSet->second.GetPathCost())
			{
				auto &mutableSet = foundClosedSet->second;
				// Update previous puzzle
				mutableSet.SetPreviousPuzzle(&valuePuzzle);
				mutableSet.SetLastMove(lastMove[i]);
				// Update costs
				mutableSet.SetPathCost(pathCostUpdated);
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
