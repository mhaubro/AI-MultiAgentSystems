/*
 * HandleGoalTask.cpp
 *
 *  Created on: 17 May 2018
 *      Author: martin
 */

#include "HandleGoalTask.h"
#include "Location.h"
#include <cmath>

HandleGoalTask::HandleGoalTask(Location loc, int rank, std::vector<bool> solvingColors, char chr)
{
	this->solvingColors = solvingColors;
	this->predecessors = std::vector<HandleGoalTask *>();
	this->chr = chr;
	//  this->type = Task::Type::MoveBoxTask;
	this->box = NULL;
	this->destination = loc;
	this->rank = rank;
	//  //std::cerr << "Compatible getColor()s for " << this->destination.first <<"," << this->destination.second <<" : " << solvinggetColor()s ;
}

HandleGoalTask::HandleGoalTask(Location loc, int rank, Box * box)
{
  this->solvingColors = std::vector<bool>();
  this->predecessors = std::vector<HandleGoalTask *>();
//  this->type = Task::Type::MoveBoxTask;
  this->box = box;
  this->chr = box->getChar();
  this->destination = loc;
  this->rank = rank;
//  //std::cerr << "Compatible getColor()s for " << this->destination.first <<"," << this->destination.second <<" : " << solvinggetColor()s ;
}

bool HandleGoalTask::isCompleted(Agent * a, Node * n)
{
	for(auto & b : n->boxes)
	{
		if(b.getChar() != this->box->getChar())
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
    if(this->box && b.getChar() != this->box->getChar())
      continue;
    if(b.getLocation() == this->destination)
      return true;
  }
  return false;
}

int HandleGoalTask::h(Agent * a, Node * n)
{
  double hval = 1000.0;

	for (Box b : n->boxes)
  {
		if (b.getColor() != a->getColor())
			continue;

		if (Goal * g = n->getGoal(b.getLocation()))
    {
			if (g->getChar() == tolower(b.getChar()))
      {
				if (g->getLocation() == b.getLocation())
        {
          if(a->getPlanner()->isFree(n, b.getLocation()))
            hval += 35;
          else
            hval -= 35;
        }
        else
          hval += 5;
			}
		}

    // The box in this task
		if (b.getChar() == box->getChar())
    {
			hval += destination.getDistance(b.getLocation());
			if (b.getDistance(n->agents[a->getChar() - '0']) < 1.3)
				hval -= 35.0;
      else
				hval += b.getDistance(n->agents[a->getChar() - '0']);
		}
	}
	return n->g()+hval;
}

bool HandleGoalTask::predecessorsComplete(Agent * a, Node * n)
{
  if(this->predecessors.size() != 0)
  {
    for(int j = 0; j < this->predecessors.size(); j++)
    {
      if(!this->predecessors[j]->seemsCompleted(a, n))
        return false;
    }
  }
  return true;
}
