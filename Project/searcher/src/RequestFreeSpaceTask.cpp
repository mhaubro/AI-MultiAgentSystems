/*
 * RequestFreeSpaceTask.cpp
 *
 *  Created on: 17 May 2018
 *      Author: martin
 */

#include "RequestFreeSpaceTask.h"

RequestFreeSpaceTask::RequestFreeSpaceTask(std::pair<int, int> loc, int rank)
{
  //this->type = Task::Type::MoveAgentTask;
  this->destination = loc;
  this->rank = rank;
}

bool RequestFreeSpaceTask::isCompleted(Agent * a, Node * n)
{
  for(auto & ag : n->agents)
  {
    if(ag.getLocation() == this->destination && ag.chr == a->chr)
    {
      a->task = nullptr;
      return true;
    }
  }
  return false;
}
bool RequestFreeSpaceTask::seemsCompleted(Agent * a, Node * n)
{
  for(auto & ag : n->agents)
  {
    if(ag.getLocation() == this->destination && ag.chr == a->chr)
    {
      return true;
    }
  }
  return false;
}


int RequestFreeSpaceTask::h(Agent * a, Node * n)
{
  return manhattan(a->getLocation(), destination);
}

