#ifndef NODE_H
#define NODE_H

#include <list>
#include <vector>
#include <string>
#include "Command.h"
#include "Goal.h"
#include "Agent.h"
#include "Box.h"

class Node {

public:
	//Fields
	static int MAX_ROW;
	static int MAX_COL;
	static std::vector<bool> walls;
	static std::vector<Goal> goals;
	std::vector<Box> boxes;
	std::vector<Agent> agents;

	Node * parent;
	Command * action;

	//Methods
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

	bool Node::isGoalState();


private:
	int gval;

	std::vector<Agent *> Node::DeepCloneAgents(std::vector<Agent *> agents)
	std::vector<Box *> Node::DeepCloneBoxes(std::vector<Box *> boxes)

	Agent * getAgent(int row, int col);
	bool Node::goalAt(int row, int col);
	bool Node::agentAt(int row, int col);
	Goal * Node::getGoal(int row, int col);
	Box * Node::getBox(int row, int col);

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
