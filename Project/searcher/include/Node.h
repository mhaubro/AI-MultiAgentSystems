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

class Goal;

class Agent;


class Node {

public:
	Node & operator =( const Node &first );
	//Fields
	static int maxX;
	static int maxY;
	static std::vector<bool> walls;
	static std::vector<Goal> goals;//Goals are read once, and order will NEVER change.
	std::vector<Box> boxes;
	std::vector<Agent> agents;
	Node * parent;
	Command * action;

	//static boost::object_pool<Node> pool;
	static Node * getopCopy(Node * n);

	static void resetPool();

	//Methods
	Node();
	Node(Node * parent);
	Node(Node * current, std::vector<Agent> * agents, std::vector<Box> * boxes);
	int g();

	bool checkState(int agent, Command * c);
	bool checkAndChangeState(int agent, Command * c);
	bool isInitialState();
	bool isGoalState();
	bool isGoalState(Entity::COLOR);
	std::vector<Node> getExpandedNodes();
	std::vector<Node> getExpandedNodes(char agent);
	std::list<Node*> extractPlan();

	std::string toString();
	int hashCode () const;
	bool equals (const Node * obj) const;
	bool operator==(const Node * obj) const;
	Box * getBox(int x, int y);
	Goal * getGoal(int x, int y);


private:
	int gval;


	//static std::vector<Agent *> DeepCloneAgents(std::vector<Agent *> agents);
	//static std::vector<Box *> DeepCloneBoxes(std::vector<Box *> boxes);

	Agent * getAgent(int x, int y);
	bool goalAt(int x, int y);
	bool agentAt(int x, int y);

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
