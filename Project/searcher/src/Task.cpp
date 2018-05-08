#include "Task.h"
#include "Agent.h"
#include "Box.h"
#include <iostream>

bool Task::isCompleted(Agent * a, Node * n)
{
  return false;
}

int Task::h(Agent * a, Node * n)
{
  return 0;
}

int Task::manhattan(std::pair<int, int> loc1, std::pair<int, int> loc2)
{
  return abs(std::get<0>(loc2) - std::get<0>(loc1)) + abs(std::get<1>(loc2) - std::get<1>(loc1));
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
      a->task = nullptr;
      return true;
    }
  }
  return false;
}

int MoveAgentTask::h(Agent * a, Node * n)
{
  return manhattan(a->getLocation(), destination);
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
      a->task = nullptr;
      return true;
    }
  }
  return false;
}

int MoveBoxTask::h(Agent * a, Node * n)
{
  for(auto & b : n->boxes)
  {
    if(b.chr == this->box->chr)
    {
      return manhattan(a->getLocation(), b.getLocation()) + manhattan(b.getLocation(), destination);
    }
  }
}
