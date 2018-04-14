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
#define RANDOM_SEED 1
#include "Goal.h"
#include "Agent.h"

bool Node::operator==(const Node * obj) const{
	return (equals(obj));
}


namespace std {
  template <> struct hash<Node *> {
    size_t operator()(const Node * n) const {
        // Some custom logic for calculating hash of CustomClass using
        // the hash Values of its individual fields

        return (n->hashCode());
    }
  };
}

	//Initialize static variables:
	int Node::MAX_ROW;
	int Node::MAX_COL;
	std::vector<bool> Node::walls;
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
		std::vector<Node *> expandedNodes = std::vector<Node *>(0);
		for (Agent * a : agents){

		for (Command * c : Command::EVERY) {
			// Determine applicability of action
			int newAgentRow = a->getY() + Command::dirToRowChange(c->dir1);
			int newAgentCol = a->getX() + Command::dirToColChange(c->dir1);

			if (c->actionType == Command::Move) {

				// Check if there's a wall or box on the cell to which the agent is moving
				if (this->cellIsFree(newAgentRow, newAgentCol)) {

					Node * n = this->ChildNode();
					n->action = c;
					n->getAgent(a->getX(), a->getY())->setLocation(std::pair<int, int>(newAgentRow, newAgentCol));
					expandedNodes.push_back(n);
				}
			} else if (c->actionType == Command::Push) {
				// Make sure that there's actually a box to move
				if (this->boxAt(newAgentRow, newAgentCol)) {
					int newBoxRow = newAgentRow + Command::dirToRowChange(c->dir2);
					int newBoxCol = newAgentCol + Command::dirToColChange(c->dir2);
					// .. and that new cell of box is free
					if (this->cellIsFree(newBoxRow, newBoxCol)) {

						Node * n = this->ChildNode();

						n->action = c;
						n->getAgent(a->getX(), a->getY())->setLocation(std::pair<int, int>(newAgentRow, newAgentCol));

						n->getBox(newAgentRow, newAgentCol)->setLocation(std::pair<int, int>(newBoxRow, newBoxCol));

						expandedNodes.push_back(n);
					}
				}
			} else if (c->actionType == Command::Pull) {
				// Cell is free where agent is going
				if (this->cellIsFree(newAgentRow, newAgentCol)) {
					int boxRow = a->getX() + Command::dirToRowChange(c->dir2);
					int boxCol = a->getY() + Command::dirToColChange(c->dir2);
					// .. and there's a box in "dir2" of the agent
					if (this->boxAt(boxRow, boxCol)) {

						Node * n = this->ChildNode();
						n->action = c;
						n->getAgent(a->getX(), a->getY())->setLocation(std::pair<int, int>(newAgentRow, newAgentCol));
						n->getBox(newAgentRow, newAgentCol)->setLocation(std::pair<int, int>(a->getX(), a->getY()));
						expandedNodes.push_back(n);
					}
				}
			}
		}
	}


		return expandedNodes;
	}


	Node * Node::ChildNode() {
		Node * copy = new Node(this);
		//This works because std::vector. Copies full 1D-array. Thank god for 1D :)
		copy->boxes = this->boxes;
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

			for (int row = 0; row < MAX_ROW; row++) {
				for (int col = 0; col < MAX_COL; col++) {
					if (Node::walls[row+col*MAX_ROW]) {
						s += ("+");
					} else {
						s += (" ");
					}
				}
			s.append("\n");
			}
			//Write a goal at location for each goal.
			for (Goal * g : goals){
				int col = g->getX();
				int row = g->getY();
				s[row+col*(MAX_ROW+1)] = g->chr;
			}
			//Write a box at location for each goal.
			for (Box * b : boxes){
				int col = b->getX();
				int row = b->getY();
				s[row+col*(MAX_ROW+1)] = b->chr;
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

	Box * Node::getBox(int row, int col)
	{
		for(Box * box : this->boxes)
		{
			if(box->getX() == row && box->getY() == col)
				return box;
		}
		return NULL;
		//throw new NullPointerException("No box at row: " + String.valueOf(row) + " and col: " + String.valueOf(col));
	}

	Goal * Node::getGoal(int row, int col)
	{
		for(Goal * goal : goals)
		{
			if(goal->getX() == row && goal->getY() == col)
				return goal;
		}
		//throw new NullPointerException("No goal at row: " + String.valueOf(row) + " and col: " + String.valueOf(col));
		return NULL;
	}


	Agent * Node::getAgent(int row, int col)
	{
		for(Agent * a : this->agents)
		{
			if(a->getX() == row && a->getY() == col)
				return a;
		}
		return NULL;
		//throw new NullPointerException("No agent at row: " + String.valueOf(row) + " and col: " + String.valueOf(col));
	}

	bool Node::cellIsFree(int row, int col)
	{
		return !walls[row + col*MAX_ROW] && !boxAt(row, col) && !agentAt(row, col);
	}

	bool Node::boxAt(int row, int col)
	{
		for(Box * box : this->boxes)
		{
			if(box->getX() == row && box->getY() == col)
				return true;
		}
		return false;
	}

	bool Node::goalAt(int row, int col)
	{
		for(Goal * goal : goals)
		{
			if(goal->getX() == row && goal->getY() == col)
				return true;
		}
		return false;
	}

	bool Node::agentAt(int row, int col)
	{
		for(Agent * a : this->agents)
		{
			if(a->getX() == row && a->getY() == col)
				return true;
		}
		return false;
	}

	std::vector<Agent*> Node::DeepCloneAgents(std::vector<Agent*> agents)
	{
		std::vector<Agent *> clone = std::vector<Agent *>(agents.size());
		for(int i = 0; i < boxes.size(); i++){
			*(clone[i]) = new Agent(*agents[i]);
		}
		return clone;
	}

	std::vector<Box *> Node::DeepCloneBoxes(std::vector<Box *> boxes)
	{
		std::vector<Box *> clone = std::vector<Box *>(boxes.size());
		for(int i = 0; i < boxes.size(); i++){
			*(clone[i]) = new Box(*boxes[i]);
		}
		return clone;
	}
