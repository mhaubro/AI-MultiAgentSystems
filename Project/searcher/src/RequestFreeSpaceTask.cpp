/*
 * RequestFreeSpaceTask.cpp
 *
 *  Created on: 17 May 2018
 *      Author: martin
 */

#include "RequestFreeSpaceTask.h"
#include <vector>

RequestFreeSpaceTask::RequestFreeSpaceTask(std::list<Location> locations, int rank, Box * serviceBox)
{
	//this->type = Task::Type::MoveAgentTask;
	this->locations = locations;
	this->rank = rank;
	this->serviceBox = serviceBox;
}
RequestFreeSpaceTask::RequestFreeSpaceTask(std::list<Location> locations, int rank)
{
	//this->type = Task::Type::MoveAgentTask;
	this->locations = locations;
	this->rank = rank;
	this->serviceBox = NULL;
}

bool RequestFreeSpaceTask::isCompleted(Agent * a, Node * n)
{
	std::list<Location>::iterator it;
	for(it = locations.begin(); it != locations.end(); ++it){
		if (Agent * agent = n->getAgent(*it)){
			if (a->getChar() == agent->getChar()){
				std::cerr << "Returning falseAgent\n";
				return false;
			}
		} else if (Box * b = n->getBox(*it)){
			if (b->getColor() == a->getColor() && (b != serviceBox)){
				return false;
			}
		}
	}

	return true;//
}

void RequestFreeSpaceTask::popLocation(){
	locations.pop_front();
}

bool RequestFreeSpaceTask::seemsCompleted(Agent * a, Node * n)
{
	return isCompleted(a, n);
}


int RequestFreeSpaceTask::h(Agent * a, Node * n)
{
	int hval = 1000.0;
	//Iterates over locations
	std::list<Location>::iterator it;
	for(it = locations.begin(); it != locations.end(); ++it){
		if (Agent * agent = n->getAgent(*it)){
			if (a->getChar() == agent->getChar()){
				hval = hval + .9;
			}
		} else if (Box * b = n->getBox(*it)){
			if (b->getColor() == a->getColor() && (b != serviceBox)){
				hval = hval + 2;
			}
		}
	}


	return hval;
}
