/*
 * HandleGoalTask.cpp
 *
 *  Created on: 17 May 2018
 *      Author: martin
 */

#include "HandleGoalTask.h"
#include <cmath>

double getDistance(Box b, std::pair<int, int> location){
	return std::sqrt(std::pow(b.getX()-location.first,2) + std::pow(b.getY()-location.second,2));
}

HandleGoalTask::HandleGoalTask(std::pair<int, int> loc, int rank, std::vector<bool> solvingColors, char chr)
{
	this->solvingColors = solvingColors;
	this->predecessors = NULL;
	this->chr = chr;
	//  this->type = Task::Type::MoveBoxTask;
	this->box = NULL;
	this->destination = loc;
	this->rank = rank;
	//  std::cerr << "Compatible colors for " << this->destination.first <<"," << this->destination.second <<" : " << solvingColors ;
}

HandleGoalTask::HandleGoalTask(std::pair<int, int> loc, int rank, Box * box)
{
  this->solvingColors = std::vector<bool>();
  this->predecessors = NULL;
//  this->type = Task::Type::MoveBoxTask;
  this->box = box;
  this->chr = box->chr;
  this->destination = loc;
  this->rank = rank;
//  std::cerr << "Compatible colors for " << this->destination.first <<"," << this->destination.second <<" : " << solvingColors ;
}

bool HandleGoalTask::isCompleted(Agent * a, Node * n)
{
	for(auto & b : n->boxes)
	{
		if(b.chr != this->box->chr)
			continue;
		if(b.getLocation() == this->destination)
		{
			return true;
		}
	}
	return false;
}

bool HandleGoalTask::seemsCompleted(Agent * a, Node * n)
{
  for(auto & b : n->boxes)
  {
    if(this->box && b.chr != this->box->chr)
      continue;
    if(b.getLocation() == this->destination)
      return true;
  }
  return false;
}

int HandleGoalTask::h(Agent * a, Node * n)
{
	double hval = 1000.0;
	//Find box.
	//Yes, the if below is legal
	//std::cerr << "Doing a heuristic thing\n";
	//Box * box;
	//IT's a moveboxtask
	//MoveBoxTask * t = dynamic_cast<MoveBoxTask *>( task );
	for (Box b : n->boxes){
		if (b.color != a->color)//We don't care
			continue;
		//For all boxes with matching color, deduce like 3 if they're on the right place
		if (Goal * g = n->getGoal(b.getX(), b.getY())){
			if (g->chr == tolower(b.chr)){
				if (g->getLocation() == b.getLocation()){
					//std::cerr << "Goal Match" << b.chr << "\n";
					hval -= 5.0;
					//std::cerr << n->toString() << "\n";
				} else {

				}
			}
		}
		if (b.chr == box->chr){
			hval += getDistance(b, destination);

			if (getDistance(b, n->agents[a->chr - '0'].getLocation()) < 1.3){//Ensures they're next to
				hval -= 5.0;
				//std::cerr << "Next to box" << b.chr << " Position: " << b.getX() <<"," <<b.getY() << "\n";
				//std::cerr << "Destination" << " Position: " << t->destination.first <<"," <<t->destination.second << "\n";
			} else {
				hval += getDistance(b, n->agents[a->chr - '0'].getLocation());
			}

			//The right box
		}
	}
	//std::cerr << "Heurestic: "<< n->g()+hval <<"\n";
	return n->g()+hval;
}
