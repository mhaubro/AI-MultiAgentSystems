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
	static std::vector<char> goals;

	std::vector<char> boxes;

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
