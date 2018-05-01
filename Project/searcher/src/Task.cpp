#include "Task.h"
#include "Agent.h"
#include "Box.h"
#include <iostream>

bool Task::isCompleted(Agent * a, Node * n)
{
  return false;
}

MoveAgentTask::MoveAgentTask(std::pair<int, int> loc, int rank)
{
  this->type = Task::Type::MoveAgentTask;
  this->destination = loc;
  this->rank = rank;
}

bool MoveAgentTask::isCompleted(Agent * a, Node * n)
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

MoveBoxTask::MoveBoxTask(Box * box, std::pair<int, int> loc, int rank)
{
  this->type = Task::Type::MoveBoxTask;
  this->box = box;
  this->destination = loc;
  this->rank = rank;
}

bool MoveBoxTask::isCompleted(Agent * a, Node * n)
{
  for(auto & b : n->boxes)
  {
    if(b.getLocation() == this->destination)
    {
      return true;
    }
  }
  return false;
}
