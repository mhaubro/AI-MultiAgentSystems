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
	Plan(std::list<Node *>);
	~Plan();
	bool checkLegality(Node * state);

	bool checkLegality(Node * state, Node * tempState);

private:
	std::list<Node *> nodes;
	std::list<Command *> actions;
	//A pair is used here to indicate the (x,y) of a new location for each command
	std::list<std::pair<int, int>> locations;//The locations that the plan will reach.

};

#endif /* SEARCHER_SRC_PLAN_H_ */
