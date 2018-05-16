/*
 * Plan.h
 *
 *  Created on: 5 May 2018
 *      Author:
 */

#ifndef PLAN_H_
#define PLAN_H_

#include "Node.h"
#include <list>
#include <utility>

class Plan {
public:
	Plan(std::list<Node *>, std::pair<int, int> startloc);
	Plan();
	~Plan();
	bool checkLegality(Node * state);

	bool checkLegality(Node * state, Node * tempState);

	bool isEmpty();

	std::string toString();

	Command * getStep();
	void popFront();
	void drain();
private:
	//std::list<Node *> nodes;
	//A pair is used here to indicate the (x,y) of a new location for each command
	std::list<std::pair<int, int>> locations;//The locations that the plan will reach.
	std::list<Command *> actions;
	std::pair<int, int> getTakenLoc(Command * c, std::pair<int, int> startloc);
	std::pair<int, int> getNewLocation(Command * c, std::pair<int, int> newloc);
};

#endif /* SEARCHER_SRC_PLAN_H_ */
