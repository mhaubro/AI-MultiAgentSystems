/*
 * HandleGoalTask.cpp
 *
 *  Created on: 17 May 2018
 *      Author: martin
 */

#include "HandleGoalTask.h"

HandleGoalTask::HandleGoalTask(Box * box, std::pair<int, int> loc, int rank)
{
//  this->type = Task::Type::MoveBoxTask;
  this->box = box;
  this->destination = loc;
  this->rank = rank;
}

bool HandleGoalTask::isCompleted(Agent * a, Node * n)
{
  for(auto & b : n->boxes)
  {
    if(b.chr != this->box->chr)
      continue;
    if(b.getLocation() == this->destination)
    {
      a->task = nullptr;
      return true;
    }
  }
  return false;
}

bool HandleGoalTask::seemsCompleted(Agent * a, Node * n)
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

int HandleGoalTask::h(Agent * a, Node * n)
{
  for(auto & b : n->boxes)
  {
    if(b.chr == this->box->chr)
    {
      return manhattan(a->getLocation(), b.getLocation()) + manhattan(b.getLocation(), destination);
    }
  }
  return 0x7FFFFFFF;
}
