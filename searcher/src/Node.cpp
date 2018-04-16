#include "Node.h"
#include <string>
#include <vector>
#include <list>
#include <ctype.h>
#include <algorithm>
#include <random>
#include <functional>
#include <iostream>
#include <iomanip>
#include "Goal.h"
#include "Agent.h"
#include <boost/pool/object_pool.hpp>

boost::object_pool<Node> Node::pool;

bool Node::operator==(const Node * obj) const{
	return (equals(obj));
}

	//Initialize static variables:
	int Node::maxX;
	int Node::maxY;
	std::vector<bool> Node::walls;
	//Maybe make it true goals instead of pointers, as this one never needs editing.
	std::vector<Goal *> Node::goals;

	//This should only be used for the first node. Inits walls & goals.

	Node::Node() {
		this->parent = NULL;
		this->gval = 0;
		this->boxes = std::vector<Box *>();
	}



	Node::Node(Node * parent) {
		this->parent = parent;
		this->gval = parent->g() + 1;
		//Inits boxes
		this->agents = DeepCloneAgents(parent->agents);
		this->boxes = DeepCloneBoxes(parent->boxes);
	}

	int Node::g() {
		return this->gval;
	}

	bool Node::isInitialState() {
		return this->parent == NULL;
	}

	std::vector<Node *> Node::getExpandedNodes(){
		std::vector<Node *> expandedNodes = std::vector<Node *>();
		for (Agent * a : agents){
			int coms = Command::EVERY.size();
			for (int i = 0; i < coms; i++) {
				Command * c = &(Command::EVERY[i]);
				// Determine applicability of action
				int newAgentX = a->getX() + Command::dirToXChange(c->dirAgent);
				int newAgentY = a->getY() + Command::dirToYChange(c->dirAgent);

				if (c->actionType == Command::Move) {

					// Check if there's a wall or box on the cell to which the agent is moving
					if (this->cellIsFree(newAgentX, newAgentY)) {
						Node * n = this->ChildNode();
						n->action = c;
						n->getAgent(a->getX(), a->getY())->setLocation(newAgentX, newAgentY);
						expandedNodes.push_back(n);
					}
				} else if (c->actionType == Command::Push) {
					// Make sure that there's actually a box to move
					if (this->boxAt(newAgentX, newAgentY)) {
						int newBoxX = newAgentX + Command::dirToXChange(c->dirBox);
						int newBoxY = newAgentY + Command::dirToYChange(c->dirBox);
						// .. and that new cell of box is free
						if (this->cellIsFree(newBoxX, newBoxY)) {
							Node * n = this->ChildNode();

							n->action = c;
							n->getAgent(a->getX(), a->getY())->setLocation(newAgentX, newAgentY);

							n->getBox(newAgentX, newAgentY)->setLocation(newBoxX, newBoxY);

							expandedNodes.push_back(n);
						}
					}
				} else if (c->actionType == Command::Pull) {
					// Cell is free where agent is going
					if (this->cellIsFree(newAgentX, newAgentY)) {
						int boxX = a->getX() + Command::dirToXChange(c->dirBox);
						int boxY = a->getY() + Command::dirToYChange(c->dirBox);
						// .. and there's a box in "dirBox" of the agent
						if (this->boxAt(boxX, boxY)) {
							Node * n = this->ChildNode();
							n->action = c;
							n->getAgent(a->getX(), a->getY())->setLocation(newAgentX, newAgentY);
							n->getBox(boxX, boxY)->setLocation(a->getX(), a->getY());
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
			for (Agent * a : agents){
				result = prime * result + a->getX();
				result = prime * result + a->getY();
			}
			//Constructs a string and hashes it. Easier than hashing random vectors.
			for (Box * b : boxes){
				result = prime * result + b->getX();
				result = prime * result + b->getY();
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
				if (!agents[i]->equals(obj->agents[i])){
					return false;
				}
			}

			//Assumes the size of boxes is the same. Maybe this should be rectified later.
			//This might be an issue. We must ensure that the order of boxes and agents are always identical.
			for (int i = 0; i < boxes.size(); i++){
				if (!boxes[i]->equals(obj->boxes[i])){
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
			for (Goal * g : goals){
				int x = g->getX();
				int y = g->getY();
				s[x+y*(maxX+1)] = g->chr;
			}
			//Write a box at location for each goal.
			for (Box * b : boxes){
				int x = b->getX();
				int y = b->getY();
				s[x+y*(maxX+1)] = b->chr;
			}

			for (Agent * a : agents){
				int x = a->getX();
				int y = a->getY();
				s[x+y*(maxX+1)] = (char) (a->num + (int) '0');
			}


		return s;
	}

	bool Node::isGoalState()
	{
		for(Goal * goal : Node::goals)
		{
			bool goalState = false;
			for(Box * box : this->boxes){
				if(goal->location == box->location){
					goalState = true;
					break;
				}
			}
		if(!goalState)
			return false;
		}
		return true;
	}

	Box * Node::getBox(int x, int y)
	{
		std::pair<int, int> loc(x,y);
		for(Box * box : this->boxes)
		{
			if(box->getLocation() == loc)
				return box;
		}
		return NULL;
	}

	Goal * Node::getGoal(int x, int y)
	{
		std::pair<int, int> loc(x,y);
		for(Goal * goal : goals)
		{
			if(goal->getLocation() == loc)
				return goal;
		}
		//throw new NullPointerException("No goal at row: " + String.valueOf(row) + " and col: " + String.valueOf(col));
		return NULL;
	}


	Agent * Node::getAgent(int x, int y)
	{
		std::pair<int, int> loc(x,y);
		for(Agent * a : this->agents)
		{
			if(a->getLocation() == loc)
				return a;
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
		for(Box * box : this->boxes)
		{
			if(box->getLocation() == loc)
				return true;
		}
		return false;
	}

	bool Node::goalAt(int x, int y)
	{
		std::pair<int, int> loc(x,y);
		for(Goal * goal : goals)
		{
			if(goal->getLocation() == loc)
				return true;
		}
		return false;
	}

	bool Node::agentAt(int x, int y)
	{
		std::pair<int, int> loc(x,y);
		for(Agent * a : this->agents)
		{
			if(a->getLocation() == loc)
				return true;
		}
		return false;
	}

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
