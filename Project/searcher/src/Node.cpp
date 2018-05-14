#include "Node.h"
#include <string>
#include <vector>
#include <list>
#include <ctype.h>
#include <algorithm>
#include <random>
#include <functional>
#include <iostream>
#include <cctype>
#include <iomanip>
#include "Goal.h"
#include "Agent.h"
#include <boost/pool/object_pool.hpp>

boost::object_pool<Node> Node::pool;

Node * Node::getopCopy(Node * n){
	return Node::pool.construct(n, &n->agents, &n->boxes);
}

Node::Node(Node * current, std::vector<Agent> * agents, std::vector<Box> * boxes){
	this->parent = current->parent;
	this->agents = *agents;
	this->boxes = *boxes;
	this->gval = current->g();
	this->action = current->action;
}

bool Node::operator==(const Node * obj) const{
	return (equals(obj));
}

Node & Node::operator=( const Node &first ){
	this->boxes = first.boxes;
	this->agents = first.agents;
	this->parent = first.parent;
	this->action = first.action;
	this->gval = first.gval;
	return *this;
}

/*
Takes an agent, takes a command and checks if it is legal
 */
bool Node::checkState(int agent, Command * c){
	//Checks for an agent command that it is legal.
	if (c->actionType == Command::Move){
		int newx = agents[agent].getX()+c->getdx(c->dirAgent);
		int newy = agents[agent].getY()+c->getdy(c->dirAgent);
		return this->cellIsFree(newx, newy);
	} else if (c->actionType == Command::Pull){
		//Calculates box' former position through knowing
		// the new position and command changes from the old.
		//Used for getting the box whose position changes
		int boxx = agents[agent].getX()+c->getdx(c->dirBox);
		int boxy = agents[agent].getY()+c->getdy(c->dirBox);
		Box * box = getBox(boxx, boxy);
		if (box == NULL)
			return false;
		int agentx = agents[agent].getX()+c->getdx(c->dirAgent);
		int agenty = agents[agent].getY()+c->getdy(c->dirAgent);
		return this->cellIsFree(agentx, agenty);
	} else if (c->actionType == Command::Push){
		int boxx = agents[agent].getX()+c->getdx(c->dirAgent);
		int boxy = agents[agent].getY()+c->getdy(c->dirAgent);
		Box * box = getBox(boxx, boxy);
		if (box == NULL)
			return false;
		int newboxx = box->getX()+c->getdx(c->dirBox);
		int newboxy = box->getY()+c->getdy(c->dirBox);
		return this->cellIsFree(newboxx, newboxy);
	} else {
		return true;
	}
}

/*
Takes an agent, executes a command and checks if it is legal. C
command:
int actionType;
int dirAgent;
int dirBox;
 */
bool Node::checkAndChangeState(int agent, Command * c){
	//Checks for legality
	if (!checkState(agent, c)){
		return false;
	}
	//Changes state
	if (c->actionType == Command::Move){
		agents[agent].setDLocation(c->getdx(c->dirAgent), c->getdy(c->dirAgent));
	} else if (c->actionType == Command::Pull){
		//Calculates box' former position through knowing
		// the new position and command changes from the old.
		//Used for getting the box whose position changes
		int boxx = agents[agent].getX()+c->getdx(c->dirBox);
		int boxy = agents[agent].getY()+c->getdy(c->dirBox);
		Box * box = getBox(boxx, boxy);
		box->setLocation(agents[agent].getLocation());
		agents[agent].setDLocation(c->getdx(c->dirAgent), c->getdy(c->dirAgent));

	} else if (c->actionType == Command::Push){
		int boxx = agents[agent].getX()+c->getdx(c->dirAgent);
		int boxy = agents[agent].getY()+c->getdy(c->dirAgent);
		Box * box = getBox(boxx, boxy);
		box->setDLocation(c->getdx(c->dirBox), c->getdy(c->dirBox));
		agents[agent].setDLocation(c->getdx(c->dirAgent), c->getdy(c->dirAgent));
	} else {
		//NoOp, do nothing
	}
	return true;
}


//Initialize static variables:
int Node::maxX;
int Node::maxY;
std::vector<bool> Node::walls;
//Maybe make it true goals instead of pointers, as this one never needs editing.
std::vector<Goal> Node::goals;

//This should only be used for the first node. Inits walls & goals.

Node::Node() {
	this->parent = NULL;
	this->gval = 0;
	this->boxes = std::vector<Box>();
}



Node::Node(Node * parent) {
	this->parent = parent;
	this->gval = parent->g() + 1;
	//Inits boxes
	this->agents = (parent->agents);
	this->boxes = (parent->boxes);
}

int Node::g() {
	return this->gval;
}

bool Node::isInitialState() {
	return this->parent == NULL;
}

std::vector<Node> Node::getExpandedNodes(){
	std::vector<Node> expandedNodes = std::vector<Node>();
	for (auto & a : agents){
		int coms = Command::EVERY.size();
		for (int i = 0; i < coms; i++) {
			Command * c = &(Command::EVERY[i]);
			// Determine applicability of action
			int newAgentX = a.getX() + Command::getdx(c->dirAgent);
			int newAgentY = a.getY() + Command::getdy(c->dirAgent);

			if (c->actionType == Command::Move) {

				// Check if there's a wall or box on the cell to which the agent is moving
				if (this->cellIsFree(newAgentX, newAgentY)) {
					Node n = Node(this);
					n.action = c;
					n.getAgent(a.getX(), a.getY())->setLocation(newAgentX, newAgentY);
					expandedNodes.push_back(n);
				}
			} else if (c->actionType == Command::Push) {
				// Make sure that there's actually a box to move
				if (this->boxAt(newAgentX, newAgentY)) {
					int newBoxX = newAgentX + Command::getdx(c->dirBox);
					int newBoxY = newAgentY + Command::getdy(c->dirBox);
					// .. and that new cell of box is free
					if (this->cellIsFree(newBoxX, newBoxY)) {
						Node n = Node(this);

						n.action = c;
						n.getAgent(a.getX(), a.getY())->setLocation(newAgentX, newAgentY);

						n.getBox(newAgentX, newAgentY)->setLocation(newBoxX, newBoxY);

						expandedNodes.push_back(n);
					}
				}
			} else if (c->actionType == Command::Pull) {
				// Cell is free where agent is going
				if (this->cellIsFree(newAgentX, newAgentY)) {
					int boxX = a.getX() + Command::getdx(c->dirBox);
					int boxY = a.getY() + Command::getdy(c->dirBox);
					// .. and there's a box in "-dirBox" of the agent
					if (this->boxAt(boxX, boxY)) {
						Node n = Node(this);
						n.action = c;
						n.getBox(boxX, boxY)->setLocation(a.getX(), a.getY());
						n.getAgent(a.getX(), a.getY())->setLocation(newAgentX, newAgentY);
						expandedNodes.push_back(n);
					}
				}
			}
		}
	}


	return expandedNodes;
}

std::vector<Node> Node::getExpandedNodes(char agent){
	std::vector<Node> expandedNodes = std::vector<Node>();
	for (auto & a : agents){
		if (a.chr != agent){
			continue;
		}

		int coms = Command::EVERY.size();
		for (int i = 0; i < coms; i++) {
			Command * c = &(Command::EVERY[i]);
			// Determine applicability of action
			int newAgentX = a.getX() + Command::getdx(c->dirAgent);
			int newAgentY = a.getY() + Command::getdy(c->dirAgent);

			if (c->actionType == Command::Move) {

				// Check if there's a wall or box on the cell to which the agent is moving
				if (this->cellIsFree(newAgentX, newAgentY)) {
					Node n = Node(this);
					n.action = c;
					n.getAgent(a.getX(), a.getY())->setLocation(newAgentX, newAgentY);
					//std::cerr << n.toString();
					expandedNodes.push_back(n);
				}
			} else if (c->actionType == Command::Push) {
				// Make sure that there's actually a box to move

				if (this->boxAt(newAgentX, newAgentY) && this->getBox(newAgentX, newAgentY)->color == a.color) {
					int newBoxX = newAgentX + Command::getdx(c->dirBox);
					int newBoxY = newAgentY + Command::getdy(c->dirBox);
					// .. and that new cell of box is free
					if (this->cellIsFree(newBoxX, newBoxY)) {
						Node n = Node(this);

						n.action = c;
						n.getAgent(a.getX(), a.getY())->setLocation(newAgentX, newAgentY);

						n.getBox(newAgentX, newAgentY)->setLocation(newBoxX, newBoxY);
						//std::cerr << n.toString();
						expandedNodes.push_back(n);
					}
				}
			} else if (c->actionType == Command::Pull) {
				// Cell is free where agent is going
				if (this->cellIsFree(newAgentX, newAgentY)) {
					int boxX = a.getX() + Command::getdx(c->dirBox);
					int boxY = a.getY() + Command::getdy(c->dirBox);
					// .. and there's a box in "dirBox" of the agent
					if (this->boxAt(boxX, boxY) && this->getBox(boxX, boxY)->color == a.color) {
						Node n = Node(this);
						n.action = c;
						n.getAgent(a.getX(), a.getY())->setLocation(newAgentX, newAgentY);
						n.getBox(boxX, boxY)->setLocation(a.getX(), a.getY());
						//std::cerr << n.toString();
						expandedNodes.push_back(n);

					}
				}
			}
		}
	}


	return expandedNodes;
}


Node * Node::ChildNode() {
	//std::cerr << "Child\n";
	Node * copy = Node::pool.construct(this);
	//This works because std::vector. Copies full 1D-array. Thank god for 1D :)
	//copy->boxes = this->boxes;
	return copy;
}

std::list<Node *> Node::extractPlan() {
	std::list<Node*> plan = std::list<Node*>();
	Node * n = this;


	while (!(n->isInitialState())) {
		plan.push_front(n);
		n = n->parent;
	}
	return plan;
}

int Node::hashCode() const{

	int prime = 31;
	int result = 1;
	for (auto & a : agents){
		result = prime * result + a.getX();
		result = prime * result + a.getY();
	}
	//Constructs a string and hashes it. Easier than hashing random vectors.
	for (auto & b : boxes){
		result = prime * result + b.getX();
		result = prime * result + b.getY();
	}

	return result;
}

bool Node::equals(const Node * obj) const {
	if (obj == NULL)
		return false;
	//Using == works with std::vector
	//Assumes the size of agents is the same. Maybe this should be rectified later.
	//This might be an issue. We must ensure that the order of boxes and agents are always identical.
	for (int i = 0; i < agents.size(); i++){
		if (!agents[i].equals(&obj->agents[i])){
			return false;
		}
	}

	//Assumes the size of boxes is the same. Maybe this should be rectified later.
	//This might be an issue. We must ensure that the order of boxes and agents are always identical.
	for (int i = 0; i < boxes.size(); i++){
		if (!boxes[i].equals(&obj->boxes[i])){
			return false;
		}
	}

	return true;
}

std::string Node::toString() {
	std::string s("");

	for (int y = 0; y < maxY; y++) {
		for (int x = 0; x < maxX; x++) {
			if (Node::walls[x+y*maxX]) {
				s += ("+");
			} else {
				s += (" ");
			}
		}
		s.append("\n");
	}
	//Write a goal at location for each goal.
	for (auto & g : goals){
		int x = g.getX();
		int y = g.getY();
		s[x+y*(maxX+1)] = g.chr;
	}
	//Write a box at location for each goal.
	for (auto & b : boxes){
		int x = b.getX();
		int y = b.getY();
		s[x+y*(maxX+1)] = b.chr;
	}

	for (auto & a : agents){
		int x = a.getX();
		int y = a.getY();
		s[x+y*(maxX+1)] = a.chr;
	}


	return s;
}

bool Node::isGoalState()
{
	for(auto & goal : Node::goals)
	{
		bool goalState = false;
		for(auto & box : this->boxes)
    {
			if(goal.location == box.location && goal.chr == std::tolower(box.chr))
      {
				goalState = true;
				break;
			}
		}
		if(!goalState){
			return false;
		}
	}
	return true;
}

bool Node::isGoalState(Entity::COLOR color)
{
	for(auto & goal : Node::goals){
		bool goalState = false;
		bool goalHasBox = false;
		for(auto & box : this->boxes) {
			if(box.color != color){
				continue;
			}
			if (std::tolower(goal.chr) == std::tolower(box.chr)){
				goalHasBox = true;
			}

			if(goal.location == box.location){
				goalState = true;
				break;
			}
		}
		if (!goalState && goalHasBox){
			//Is Okay
			return false;
		}
	}
	return true;
}



Box * Node::getBox(int x, int y)
{
	std::pair<int, int> loc(x,y);
	for(auto & box : this->boxes)
	{
		if(box.getLocation() == loc)
			return &box;
	}
	return NULL;
}

Goal * Node::getGoal(int x, int y)
{
	std::pair<int, int> loc(x,y);
	for(auto & goal : goals)
	{
		if(goal.getLocation() == loc)
			return &goal;
	}
	//throw new NullPointerException("No goal at row: " + String.valueOf(row) + " and col: " + String.valueOf(col));
	return NULL;
}


Agent * Node::getAgent(int x, int y)
{
	std::pair<int, int> loc(x,y);
	for(auto & a : this->agents)
	{
		if(a.getLocation() == loc)
			return &a;
	}
	return NULL;
	//throw new NullPointerException("No agent at row: " + String.valueOf(row) + " and col: " + String.valueOf(col));
}

bool Node::cellIsFree(int x, int y)
{
	return !walls[x + y*maxX] && !boxAt(x, y) && !agentAt(x, y);
}

bool Node::boxAt(int x, int y)
{
	std::pair<int, int> loc(x,y);
	for(auto & box : this->boxes)
	{
		if(box.getLocation() == loc)
			return true;
	}
	return false;
}

bool Node::goalAt(int x, int y)
{
	std::pair<int, int> loc(x,y);
	for(auto & goal : goals)
	{
		if(goal.getLocation() == loc)
			return true;
	}
	return false;
}

bool Node::agentAt(int x, int y)
{
	std::pair<int, int> loc(x,y);
	for(auto & a : this->agents)
	{
		if(a.getLocation() == loc)
			return true;
	}
	return false;
}
/*
	std::vector<Agent*> Node::DeepCloneAgents(std::vector<Agent*> agents)
	{
		std::vector<Agent *> clone (agents.size());
		for(int i = 0; i < agents.size(); i++){
			//std::cerr << "Creating agent\n";
			clone[i] = Agent::pool.construct(agents[i]);
		}
		return clone;
	}

	std::vector<Box*> Node::DeepCloneBoxes(std::vector<Box *> boxes)
	{
		std::vector<Box *> clone (boxes.size());
		for(int i = 0; i < boxes.size(); i++){
			//std::cerr << "Creating box\n";
			clone[i] = Box::pool.construct(boxes[i]);
		}
		return clone;
	}
 */
