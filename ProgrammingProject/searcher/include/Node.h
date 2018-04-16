#ifndef NODE_H
#define NODE_H

#include <list>
#include <vector>
#include <string>
#include "Command.h"
#include "Goal.h"
#include "Agent.h"
#include "Box.h"
#include <boost/pool/object_pool.hpp>


class Node {

public:
	//Fields
	static int maxX;
	static int maxY;
	static std::vector<bool> walls;
	static std::vector<Goal *> goals;
	std::vector<Box *> boxes;
	std::vector<Agent *> agents;

	static boost::object_pool<Node> pool;

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

private:
	int gval;


	static std::vector<Agent *> DeepCloneAgents(std::vector<Agent *> agents);
	static std::vector<Box *> DeepCloneBoxes(std::vector<Box *> boxes);

	Agent * getAgent(int x, int y);
	bool goalAt(int x, int y);
	bool agentAt(int x, int y);
	Goal * getGoal(int x, int y);
	Box * getBox(int x, int y);

	bool cellIsFree(int x, int y);
	bool boxAt(int x, int y);
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
