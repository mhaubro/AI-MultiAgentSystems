#ifndef NODE_H
#define NODE_H

#include <list>
#include <vector>
#include <string>
#include "Command.h"
#include "Goal.h"
#include "Agent.h"
#include "Box.h"
#include <iostream>
#include "Location.h"

class Goal;

class Agent;

class Node {

public:
	Node & operator =( const Node &first );

	void removeBox(Location loc);
	void removeAgent(Location loc);
	//Fields
	static int maxX;
	static int maxY;
	static std::vector<bool> walls;
	static std::vector<Goal> goals;//Goals are read once, and order will NEVER change.
	std::vector<Box> boxes;
	std::vector<Agent> agents;

	//static boost::object_pool<Node> pool;
	static Node * getopCopy(Node * n);

	static void resetPool();

	void clearOtherAgents(char agent);
	void clearOtherAgentsAndBoxes(char agent, Box * box);
	void clearGoals(char agent, Goal * g1, Goal * g2);

  //Methods
	Node();
	Node(Node * parent, Command * action);
	Node(Node * current, std::vector<Agent> * agents, std::vector<Box> * boxes);
	int g() const;

	bool checkState(int agent, Command * c);
	bool checkAndChangeState(int agent, Command * c);
	bool isInitialState();
	bool isGoalState();
	bool isGoalState(Entity::COLOR);
    bool isGoalState(Goal * g);
	std::vector<Node> getExpandedNodes();
	std::vector<Node> getExpandedNodes(char agent);
  std::vector<Node> getExpandedNodes(char agent, Goal * g);
	std::list<Node*> extractPlan();

	void clearOtherRegions(int regionToRemain);

	std::string toString();
	int hashCode () const;
	bool equals (const Node * obj) const;
	bool operator==(const Node * obj) const;
	Box * getBox(Location location);
	Goal * getGoal(Location location);
	Agent * getAgent(Location location);

	Node * getParent() const;
	Command * getAction() const;

  std::vector<Location> recordAgentLocations();
  void resetAgent(std::vector<Location> locations);
  void solveGoal(Goal * g);
  bool goalAt(Location location);

private:

	Location getBoxLocation(Agent * agent, Command * c);
	int gval;
	const Node * parent;
	const Command * action;

	bool agentAt(Location location);

	bool cellIsFree(Location location);
	bool boxAt(Location location);
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

//This is the running state node. Will never be deleted, and always have g = 0, and parent == NULL.
extern Node stateNode;

#endif
