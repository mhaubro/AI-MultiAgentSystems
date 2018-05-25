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
	std::list<Location>::iterator it;

	//std::cerr << "New RequestFreeSpaceTask created over locations: \n";
	for(it = locations.begin(); it != locations.end(); ++it){
		//std::cerr << *it<< ", ";
	}
	//std::cerr<< "\n";
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
				////std::cerr << "Returning falseAgent\n";
				return false;
			}
		} else if (Box * b = n->getBox(*it)){
			if (b->getColor() == a->getColor() && serviceBox && (b->getChar() != serviceBox->getChar())){
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
	bool isBoxOnPath = false;
	Agent * agent;
	for (int i = 0; i < n->agents.size(); i++){
		if (a->getID() == n->agents[i].getID()){
			agent = &n->agents[i];
			break;
		}
	}

	for(it = locations.begin(); it != locations.end(); ++it){
		if (Box * b = n->getBox(*it)){
			if (b->getColor() == a->getColor() && serviceBox && (b->getChar() != serviceBox->getChar())){
				hval += 1 + b->getDistance(*agent);
			}
		}
	}

	if (!isBoxOnPath){
		hval -= 20.0;//This is a really good thing
		for(it = locations.begin(); it != locations.end(); ++it){
			if (*it == agent->getLocation()){
				hval = hval + .9;
			}
		}
	}



	return hval;
}
