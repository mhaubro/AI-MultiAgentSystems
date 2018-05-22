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
#include "ManualNodePool.h"
//#include <boost/pool/object_pool.hpp>
//Very possibly we should do our own, considering we don't have to "destruct" node *.
//boost::object_pool<Node> Node::pool;

Node stateNode;

void Node::clearOtherAgentsAndBoxes(char agent, Box * box){
	std::vector<Agent> newA = std::vector<Agent>();
	std::vector<Box> newB = std::vector<Box>();
	for (const auto & a : agents){
		if (a.getChar() == agent){
			newA.emplace_back(&a);
		}
	}
	newB.push_back(getBox(box->getLocation()));
	agents = newA;
	boxes = newB;
}


void Node::clearOtherAgents(char agent){
	std::vector<Agent> newA = std::vector<Agent>();
	for (const  auto & a : agents){
		if (a.getChar() == agent){
			newA.emplace_back(&a);
		}
	}
	agents = newA;
}

void Node::resetPool(){
	pool.clearNodes();
}

Node * Node::getopCopy(Node * n){
	////std::cerr << "creating Node copy\n";
	return pool.createNodeCopy(n, n->agents, n->boxes);
}

Node::Node(Node * current, std::vector<Agent> * agents, std::vector<Box> * boxes)
: parent(current->getParent()), gval(current->g()), action(current->getAction())
{
	this->parent = current->parent;
	this->agents = *agents;
	this->boxes = *boxes;
}

bool Node::operator==(const Node * obj) const{
	return (equals(obj));
}

Node & Node::operator=( const Node &first ){
	this->boxes = first.boxes;
	this->agents = first.agents;
	this->parent = first.getParent();
	this->action = first.getAction();
	this->gval = first.g();
	return *this;
}

/*
 * Takes an agent * and a command, and calculates where there must be a moveable box.
 */
Location Node::getBoxLocation(Agent * agent, Command * c){
	if (c->getActionType() == Command::Pull){
		return agent->getLocation() - c->boxdloc();
	} else if (c->getActionType() == Command::Push){
		return agent->getLocation() + c->boxdloc();
	} else {
		throw "getBoxCalled Wrongly\n";
		return Location(-5, -5);
	}
}

/*
Takes an agent, takes a command and checks if it is legal
 */

bool Node::checkState(int agent, Command * c){
	Agent * activeAgent = &agents[agent];
	//Checks for an agent command that it is legal.
	if (c->getActionType() == Command::Move){
		Location newL = activeAgent->getLocation() + c->agentdloc();
		return this->cellIsFree(newL);
	} else if (c->getActionType() == Command::Pull){
		Location boxL = getBoxLocation(activeAgent, c);
		Box * box = getBox(boxL);
		if (box == NULL || box->getColor() != activeAgent->getColor())
			return false;
		Location agentL = activeAgent->getLocation() + c->agentdloc();

		return this->cellIsFree(agentL);
	} else if (c->getActionType() == Command::Push){
		Location boxloc = getBoxLocation(activeAgent, c);
		Box * box = getBox(boxloc);
		if (box == NULL || box->getColor() != activeAgent->getColor())
			return false;
		boxloc = box->getLocation() + c->boxdloc();
		return this->cellIsFree(boxloc);
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
	Agent * activeAgent = &agents[agent];
	//Checks for legality
	//if (c == )
	if (!checkState(agent, c)){
		return false;
	}
	//Changes state
	if (c->getActionType() == Command::Move){
		activeAgent->setDLocation(c->agentdloc());

	} else if (c->getActionType() == Command::Pull){
		//Calculates box' former position through knowing
		// the new position and command changes from the old.
		//Used for getting the box whose position changes
		Location boxloc = getBoxLocation(activeAgent, c);
		Box * box = getBox(boxloc);
		box->setLocation(activeAgent->getLocation());
		activeAgent->setDLocation(c->agentdloc());

	} else if (c->getActionType() == Command::Push){
		Location boxloc = getBoxLocation(activeAgent, c);
		Box * box = getBox(boxloc);
		box->setDLocation(c->boxdloc());
		activeAgent->setDLocation(c->agentdloc());

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

Node::Node() : gval(0) {
	this->parent = NULL;
	this->boxes = std::vector<Box>();
	action = NULL;
}

Node::Node(Node * parent, Command * c) : gval(parent->g()+1), action(c) {
	this->parent = parent;
	//Inits boxes
	this->agents = (parent->agents);
	this->boxes = (parent->boxes);
	this->action = c;
}

int Node::g() const{
	return this->gval;
}

bool Node::isInitialState() {
	return this->parent == NULL;
}

std::vector<Node> Node::getExpandedNodes(char agent){
	std::vector<Node> expandedNodes = std::vector<Node>();
	for (const auto & a : agents){

		if (a.getChar() != agent){
			continue;
		}

		Location agentLoc = a.getLocation();
		int coms = Command::EVERY.size();
		for (int i = 0; i < coms; i++) {
			Command * c = &(Command::EVERY[i]);
			// Determine applicability of action
			Location newAgentLoc = a.getLocation() + c->agentdloc();

			if (c->getActionType() == Command::Move) {

				// Check if there's a wall or box on the cell to which the agent is moving
				if (this->cellIsFree(newAgentLoc)) {
					Node n = Node(this, c);

					n.getAgent(agentLoc)->setLocation(newAgentLoc);

					expandedNodes.push_back(n);
				}
			} else if (c->getActionType() == Command::Push) {
				// Make sure that there's actually a box to move
				if (this->boxAt(newAgentLoc) && this->getBox(newAgentLoc)->getColor() == a.getColor()) {
					Location newBoxLoc = newAgentLoc + c->agentdloc();
					//std::cerr << newBoxLoc.toString() << "\n";
					// .. and that new cell of box is free
					if (this->cellIsFree(newBoxLoc)) {
						Node n = Node(this, c);

						n.getAgent(agentLoc)->setLocation(newAgentLoc);

						n.getBox(newAgentLoc)->setLocation(newBoxLoc);

						expandedNodes.push_back(n);
					}
				}
			} else if (c->getActionType() == Command::Pull) {
				// Cell is free where agent is going
				if (this->cellIsFree(newAgentLoc)) {
					Location boxLoc = agentLoc - c->boxdloc();

					if (this->boxAt(boxLoc) && this->getBox(boxLoc)->getColor() == a.getColor()) {
						Node n = Node(this, c);
						n.getBox(boxLoc)->setLocation(agentLoc);
						n.getAgent(agentLoc)->setLocation(newAgentLoc);
						expandedNodes.push_back(n);
					}
				}
			}
		}
	}
	return expandedNodes;
}


std::list<Node *> Node::extractPlan()
{
	std::list<Node*> plan = std::list<Node*>();
	Node * n = this;

	while (!(n->isInitialState())) {
		plan.push_front(n);
		n = n->getParent();
	}
	return plan;
}

Node * Node::getParent() const {
	return (Node *) parent;
}
Command * Node::getAction() const {
	return (Command *) action;
}



int Node::hashCode() const
{

	int prime = 31;
	int result = 1;
	for (auto & a : agents){
		result = prime * result + a.getLocation().getX();
		result = prime * result + a.getLocation().getY();
	}
	//Constructs a string and hashes it. Easier than hashing random vectors.
	for (auto & b : boxes){
		result = prime * result + b.getLocation().getX();
		result = prime * result + b.getLocation().getY();
	}
	////std::cerr << "Calling hash with val" << result << "\n";
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
	////std::cerr << "Objects are equal\n";
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
		int x = g.getLocation().getX();
		int y = g.getLocation().getY();
		s[x+y*(maxX+1)] = g.getChar();
	}
	//Write a box at location for each goal.
	for (auto & b : boxes){
		int x = b.getLocation().getX();
		int y = b.getLocation().getY();
		s[x+y*(maxX+1)] = b.getChar();
	}

	for (auto & a : agents){
		int x = a.getLocation().getX();
		int y = a.getLocation().getY();
		s[x+y*(maxX+1)] = a.getChar();
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
			if(goal.getLocation() == box.getLocation() && goal.getChar() == std::tolower(box.getChar()))
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
			if(box.getColor() != color){
				continue;
			}
			if (std::tolower(goal.getChar()) == std::tolower(box.getChar())){
				goalHasBox = true;
			}

			if(goal.getLocation() == box.getLocation()){
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

bool Node::isGoalState(Goal g)
{
  for(auto & b : this->boxes)
  {
    if(g.getLocation() == b.getLocation() && g.getChar() == std::tolower(b.getChar()))
    {
      return true;
    }
  }
	return false;
}

Box * Node::getBox(Location loc)
{
	for(auto & box : this->boxes)
	{
		if(box.getLocation() == loc)
			return &box;
	}
	return NULL;
}

Goal * Node::getGoal(Location loc)
{
	for(auto & goal : goals)
	{
		if(goal.getLocation() == loc)
			return &goal;
	}
	//throw new NullPointerException("No goal at row: " + String.valueOf(row) + " and col: " + String.valueOf(col));
	return NULL;
}


Agent * Node::getAgent(Location loc)
{
	for(auto & a : this->agents)
	{
		if(a.getLocation() == loc)
			return &a;
	}
	return NULL;
	//throw new NullPointerException("No agent at row: " + String.valueOf(row) + " and col: " + String.valueOf(col));
}

bool Node::cellIsFree(Location loc)
{
	return !walls[loc.getX() + loc.getY()*maxX] && !boxAt(loc) && !agentAt(loc);
}

bool Node::boxAt(Location loc)
{
	for(auto & box : this->boxes)
	{
		if(box.getLocation() == loc)
			return true;
	}
	return false;
}

bool Node::goalAt(Location loc)
{
	for(auto & goal : goals)
	{
		if(goal.getLocation() == loc)
			return true;
	}
	return false;
}

bool Node::agentAt(Location loc)
{
	for(auto & a : this->agents)
	{
		if(a.getLocation() == loc)
			return true;
	}
	return false;
}
