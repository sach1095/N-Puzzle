#ifndef ENUM_HPP
#define ENUM_HPP

//TODO Implement greedy search and uniform_cost search
enum Algorithm {
	ASTAR = 0,
	GREEDY = 1,
	UNIFORM_COST = 2,
};

enum Heuristics {
	MANHATTAN = 0,
	// TODO a implementer
	OTHER = 1,
	OTHERTWO = 2,
};

enum Move {
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3,
	NONE = 4,
};

#endif
