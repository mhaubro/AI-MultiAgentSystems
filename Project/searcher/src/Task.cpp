#include "Task.h"
#include "Agent.h"
#include "Box.h"

Task::Task()
{

}

int Task::h()
{
  return 0;
}

int Task::manhattan(std::pair<int, int> loc1, std::pair<int, int> loc2)
{
  return abs(std::get<0>(loc2) - std::get<0>(loc1)) + abs(std::get<1>(loc2) - std::get<1>(loc1));
}

MoveAgentTask::MoveAgentTask(Agent * agent, std::pair<int, int> loc, int rank)
{
  this->agent = agent;
  this->destination = loc;
  this->rank = rank;
}

int MoveAgentTask::h()
{
  return manhattan(this->agent->location, this->destination);
}

MoveBoxTask::MoveBoxTask(Agent * agent, Box * box, std::pair<int, int> loc, int rank)
{
  this->agent = agent;
  this->box = box;
  this->destination = loc;
  this->rank = rank;
}

int MoveBoxTask::h()
{
  return manhattan(this->agent->location, this->destination) + manhattan(this->box->location, this->destination);
}
