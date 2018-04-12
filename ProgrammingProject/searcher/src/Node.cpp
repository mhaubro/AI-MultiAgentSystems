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
	std::vector<char> Node::goals;

	//This should only be used for the first node. Inits walls & goals.

	Node::Node() {
		this->parent = NULL;
		this->gval = 0;
		this->boxes = std::vector<char>(MAX_COL*MAX_ROW, '\0');
		this->agentCol = 0;
		this->agentRow = 0;
	}



	Node::Node(Node * parent) {
		this->parent = parent;
		this->gval = parent->g() + 1;
		this->agentCol = parent->agentCol;
		this->agentRow = parent->agentRow;
		//Inits boxes
		this->boxes = std::vector<char>(MAX_COL*MAX_ROW, '\0');
	}

	int Node::g() {
		return this->gval;
	}

	bool Node::isInitialState() {
		return this->parent == NULL;
	}

	std::vector<Node *> Node::getExpandedNodes(){
		std::vector<Node *> expandedNodes = std::vector<Node *>(0);
		for (Command * c : Command::EVERY) {
			// Determine applicability of action
			int newAgentRow = this->agentRow + Command::dirToRowChange(c->dir1);
			int newAgentCol = this->agentCol + Command::dirToColChange(c->dir1);

			if (c->actionType == Command::Move) {

				// Check if there's a wall or box on the cell to which the agent is moving
				if (this->cellIsFree(newAgentRow, newAgentCol)) {

					Node * n = this->ChildNode();
					n->action = c;
					n->agentRow = newAgentRow;
					n->agentCol = newAgentCol;
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
						n->agentRow = newAgentRow;
						n->agentCol = newAgentCol;
						n->boxes[newBoxRow+newBoxCol*MAX_ROW] = this->boxes[newAgentRow+newAgentCol*MAX_ROW];
						n->boxes[newAgentRow+newAgentCol*MAX_ROW] = 0;
						expandedNodes.push_back(n);
					}
				}
			} else if (c->actionType == Command::Pull) {
				// Cell is free where agent is going
				if (this->cellIsFree(newAgentRow, newAgentCol)) {
					int boxRow = this->agentRow + Command::dirToRowChange(c->dir2);
					int boxCol = this->agentCol + Command::dirToColChange(c->dir2);
					// .. and there's a box in "dir2" of the agent
					if (this->boxAt(boxRow, boxCol)) {

						Node * n = this->ChildNode();
						n->action = c;
						n->agentRow = newAgentRow;
						n->agentCol = newAgentCol;
						n->boxes[this->agentRow+this->agentCol*MAX_ROW] = this->boxes[boxRow+boxCol*MAX_ROW];
						n->boxes[boxRow+boxCol*MAX_ROW] = 0;
						expandedNodes.push_back(n);
					}
				}
			}
		}

		return expandedNodes;
	}

	bool Node::boxAt(int row, int col) {
		return boxes[row+col*MAX_ROW] > 0;
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
			result = prime * result + this->agentCol;
			result = prime * result + this->agentRow;
			//Constructs a string and hashes it. Easier than hashing random vectors.
			std::string str(this->boxes.begin(),this->boxes.end());
			result = prime * result + std::hash<std::string>{}(str);;
			//str = std::string(this->goals.begin(),this->goals.end());
			//result = prime * result + std::hash<std::string>{}(str);
			result = prime * result + std::hash<std::vector<bool>>{}(this->walls);

			return result;
	}

	bool Node::equals(const Node * obj) const {
		if (obj == NULL)
			return false;
			//Using == works with std::vector
		if (this->agentRow != obj->agentRow || this->agentCol != obj->agentCol)
			return false;
		if ((this->boxes != obj->boxes))
			return false;
		return true;
	}

	std::string Node::toString() {
		std::string s("");

			for (int row = 0; row < MAX_ROW; row++) {
				for (int col = 0; col < MAX_COL; col++) {

				if (Node::boxes[row+col*MAX_ROW] != '\0') {
					s += (boxes[row+col*MAX_ROW]);
				} else if (Node::goals[row+col*MAX_ROW] != '\0') {
					s += (goals[row+col*MAX_ROW]);
				} else if (Node::walls[row+col*MAX_ROW]) {
					s += ("+");
				} else if (row == this->agentRow && col == this->agentCol) {
					s += ("0");
				} else {
					s += (" ");
				}
			}
			s.append("\n");
		}
		return s;
	}

	bool Node::isGoalState()
	{
		for(Goal * goal : Node->goals)
		{
			bool goalState = false;
			for(Box * box : this->boxes){
				if(goal->location->equals(box->location)){
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
			if(box->location.getLeft() == row && box->location.getRight() == col)
				return box;
		}
		return NULL;
		//throw new NullPointerException("No box at row: " + String.valueOf(row) + " and col: " + String.valueOf(col));
	}

	Goal * Node::getGoal(int row, int col)
	{
		for(Goal * goal : goals)
		{
			if(goal->location.getLeft() == row && goal->location.getRight() == col)
				return goal;
		}
		//throw new NullPointerException("No goal at row: " + String.valueOf(row) + " and col: " + String.valueOf(col));
		return NULL;
	}


	Agent * Node::getAgent(int row, int col)
	{
		for(Agent * a : this->agents)
		{
			if(a->location.getLeft() == row && a->location.getRight() == col)
				return a;
		}
		return NULL;
		//throw new NullPointerException("No agent at row: " + String.valueOf(row) + " and col: " + String.valueOf(col));
	}

	bool Node::cellIsFree(int row, int col)
	{
		return !walls[row][col] && !boxAt(row, col) && !agentAt(row, col);
	}

	bool Node::boxAt(int row, int col)
	{
		for(Box * box : this->boxes)
		{
			if(box->location.getLeft() == row && box->location.getRight() == col)
				return true;
		}
		return false;
	}

	bool Node::goalAt(int row, int col)
	{
		for(Goal * goal : goals)
		{
			if(goal->location.getLeft() == row && goal->location.getRight() == col)
				return true;
		}
		return false;
	}

	bool Node::agentAt(int row, int col)
	{
		for(Agent * a : this->agents)
		{
			if(a->location.getLeft() == row && a->location.getRight() == col)
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
