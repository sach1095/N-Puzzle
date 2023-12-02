#ifndef ZOBRISTHASH_HPP
# define ZOBRISTHASH_HPP

#include <iostream>
#include <limits>
#include <random>
#include <vector>
#include "Custom_Error.hpp"

// https://en.wikipedia.org/wiki/Zobrist_hashing
class ZobristHash
{
public:
	ZobristHash(size_t numberPieces): Size(numberPieces)
	{
		this->Table.reserve(numberPieces);

		// Fill with random bitset
		std::random_device rd;
		std::mt19937 gen(rd());
		int min_value = std::numeric_limits<int>::min();
		int max_value = std::numeric_limits<int>::max();
		std::uniform_int_distribution<int> distribution(min_value, max_value);

		for (size_t i = 0; i < numberPieces; ++i)
		{
			std::vector<int> vec(numberPieces);
			for (size_t j = 0; j < numberPieces; ++j)
				vec[j] = distribution(gen);
			this->Table.push_back(vec);
		}
	}

	int Hash(const std::vector<int>& vector) const
	{
		if (vector.size() != this->Size)
			throw CustomError("ZobristHash : Wrong size");

		int hash = 0;
		for (size_t i = 0; i < vector.size(); ++i)
			hash = hash ^ this->Table[i][vector[i]];
		return hash;
	}

	// Recompute the hash after swapping the piece1 at the pose1
	// and the piece2 at the pose2
	int Swap(int prevHash, int pose1, int piece1, int pose2, int piece2) const
	{
		int hash = prevHash;
		// Replace piece1 by piece2 at pose1
		hash = hash ^ this->Table[pose1][piece1];
		hash = hash ^ this->Table[pose1][piece2];
		// Replace piece2 by piece1 at pose2
		hash = hash ^ this->Table[pose2][piece2];
		hash = hash ^ this->Table[pose2][piece1];

		return hash;
	}

private:
	size_t Size;
	std::vector<std::vector<int>> Table;
};

#endif