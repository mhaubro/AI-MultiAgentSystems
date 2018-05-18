/*
 * RequestFreeSpaceTask.cpp
 *
 *  Created on: 17 May 2018
 *      Author: martin
 */

#include "RequestFreeSpaceTask.h"
#include <vector>

RequestFreeSpaceTask::RequestFreeSpaceTask(std::list<std::pair<int, int>> locations, int rank)
{
	//this->type = Task::Type::MoveAgentTask;
	this->locations = locations;
	this->rank = rank;
}

bool RequestFreeSpaceTask::isCompleted(Agent * a, Node * n)
{
	std::list<std::pair<int, int>>::iterator it;
	for(it = locations.begin(); it != locations.end(); ++it){
		if (Agent * agent = n->getAgent((*it).first, (*it).second)){
			if (a->chr == agent->chr){
				return false;
			}
		} else if (Box * b = n->getBox((*it).first, (*it).second)){
			if (b->color == a->color){
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
	std::list<std::pair<int, int>>::iterator it;
	for(it = locations.begin(); it != locations.end(); ++it){
		if (Agent * agent = n->getAgent((*it).first, (*it).second)){
			if (a->chr == agent->chr){
				hval = hval - .9;
			}
		} else if (Box * b = n->getBox((*it).first, (*it).second)){
			if (b->color == a->color){
				hval = hval - .9;
			}
		}
	}


	return 0.0;
}

