#include "Task.h"
#include "Agent.h"
#include "Box.h"

MoveAgentTask::MoveAgentTask(Agent * agent, std::pair<int, int> loc, int rank)
{
  this->agent = agent;
  this->destination = loc;
  this->rank = rank;
}

MoveBoxTask::MoveBoxTask(Box * box, std::pair<int, int> loc, int rank)
{
  this->box = box;
  this->destination = loc;
  this->rank = rank;
}
