#ifndef NODE_H
#define NODE_H

#include "centralHeader.h"


class Node {

public:
	static int MAX_ROW = 70;
	static int MAX_COL = 70;

	int agentRow;
	int agentCol;
	Node(Node * parent);
	int g();
	bool isInitialState();
	bool isGoalState();
	std::vector getExpandedNodes();
	std::list<Node> extractPlan();
	int hashCode();
	bool equals(Object * obj);
	std::string toString();

	// Arrays are indexed from the top-left of the level, with first index being row and second being column.
	// Row 0: (0,0) (0,1) (0,2) (0,3) ...
	// Row 1: (1,0) (1,1) (1,2) (1,3) ...
	// Row 2: (2,0) (2,1) (2,2) (2,3) ...
	// ...
	// (Start in the top left corner, first go down, then go right)
	// E.g. this.walls[2] is an array of booleans having size MAX_COL.
	// this.walls[row][col] is true if there's a wall at (row, col)
	//

	static std::vector<bool> walls;
	std::vector<char> boxes;
	static std::vector<char> goals;

	Node parent;
	Command action;

private:
	int g;
	int _hash = 0;
	bool cellIsFree(int row, int col)
	bool boxAt(int row, int col)
	Node ChildNode()

}

#endif
