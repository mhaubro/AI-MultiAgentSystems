/*
 * Plan.cpp
 *
 *  Created on: 5 May 2018
 *      Author:
 */

#include "Plan.h"

#define LOOKAHEAD 1

Plan::Plan(){
actions = std::list<Command *>();
}

Plan::Plan(std::list<Node *> nodes, std::pair<int, int> startloc) {
	// TODO Auto-generated constructor stub
	actions = std::list<Command *>();
	locations = std::list<std::pair<int, int>>();
	while (!nodes.empty()){
		actions.push_back(nodes.front()->action);
		nodes.pop_front();
	}

	std::pair<int, int> newAgentloc = startloc;

	//Gets locations
	std::list<Command *>::iterator it;
	for (it = actions.begin(); it != actions.end(); ++it){
		startloc = newAgentloc;
		newAgentloc = getNewLocation(*it, newAgentloc);
		std::pair<int, int> newTakenLoc = getTakenLoc(*it, startloc);
	    locations.push_back(newTakenLoc);
	}

	//Maybe grab and store location
	//TODO


}

std::pair<int, int> Plan::getTakenLoc(Command * c, std::pair<int, int> startloc){



	return std::pair<int, int>(0, 0);
}
std::pair<int, int> Plan::getNewLocation(Command * c, std::pair<int, int> newloc){



	return std::pair<int, int>(0, 0);
}


std::string Plan::toString(){
	std::string s;
	for (Command * c : actions){
		s.append(c->toString());
	}
	return s;
}


Plan::~Plan() {
	// TODO Auto-generated destructor stub
}

bool Plan::isEmpty(){
	return actions.empty();
}

Command * Plan::getStep(){
	Command * c = actions.front();
	actions.pop_front();
	return c;
}

void Plan::popFront(){
	actions.pop_front();
}
void Plan::drain(){
	actions.clear();
}

//Checks if the plan is legal with the current lookahead.
//An undefined lookahead (Outcomment top) will lead to checking the entire plan
bool Plan::checkLegality(Node * state){
	//Not necessary at the moment


	return true;
}

bool Plan::checkLegality(Node * state, Node * tempState){
	if (state == tempState){
		return checkLegality(state);
	} else {
		return (checkLegality(state) && checkLegality(tempState));
	}
}



