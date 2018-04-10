#ifndef NODE_H
#define NODE_H

#include <list>
#include <vector>
#include <string>
#include "Command.h"

class Node {

public:
	static int MAX_ROW;
	static int MAX_COL;
	static std::vector<bool> walls;
	std::vector<char> boxes;
	static std::vector<char> goals;

	int agentRow;
	int agentCol;

	Node * parent;
	Command * action;

	Node();
	Node(Node * parent);
	int g();
	bool isInitialState();
	bool isGoalState();
	std::vector<Node*> getExpandedNodes();
	std::list<Node*> extractPlan();
	std::string toString();

	int hashCode () const;
	bool equals (const Node * obj) const;
	bool operator==(const Node * obj) const;
	// Arrays are indexed from the top-left of the level, with first index being row and second being column.
	// Row 0: (0,0) (0,1) (0,2) (0,3) ...
	// Row 1: (1,0) (1,1) (1,2) (1,3) ...
	// Row 2: (2,0) (2,1) (2,2) (2,3) ...
	// ...
	// (Start in the top left corner, first go down, then go right)
	// E.g. this.walls[2] is an array of booleans having size MAX_COL.
	// this.walls[row][col] is true if there's a wall at (row, col)
	//



private:
	int gval;
	int _hash = 0;
	bool cellIsFree(int row, int col);
	bool boxAt(int row, int col);
	Node * ChildNode();

};
/*
* Necessary for hashing using the std::unordered set
*/
struct NodeHash {
public:
	size_t operator()(const Node * n) const {
		return n->hashCode();
	}
};

/*
* Necessary for comparing using the std::unordered set
*/
struct NodeEqual {
public:
	bool operator()(const Node * n1, const Node * n2) const {

		return n1->equals(n2);
	}
};

#endif
