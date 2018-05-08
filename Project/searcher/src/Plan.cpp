/*
 * Plan.cpp
 *
 *  Created on: 5 May 2018
 *      Author:
 */

#include "Plan.h"

#define LOOKAHEAD 1


Plan::Plan(std::list<Node *>) {
	// TODO Auto-generated constructor stub

}

Plan::~Plan() {
	// TODO Auto-generated destructor stub
}

//Checks if the plan is legal with the current lookahead.
//An undefined lookahead (Outcomment top) will lead to checking the entire plan
bool Plan::checkLegality(Node * state){



	return true;
}

bool Plan::checkLegality(Node * state, Node * tempState){
	if (state == tempState){
		return checkLegality(state);
	} else {
		return (checkLegality(state) && checkLegality(tempState));
	}
}



