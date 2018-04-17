#ifndef TASK_H
#define TASK_H

#include "Agent.h"
#include "Box.h"

class Agent;

class MoveAgentTask
{
  public:
    Agent * agent;
    std::pair<int, int> destination;
    int rank;

    MoveAgentTask(Agent * agent, std::pair<int, int> loc, int rank);
};

class MoveBoxTask
{
  public:
    Box * box;
    std::pair<int, int> destination;
    int rank;

    MoveBoxTask(Box * box, std::pair<int, int> loc, int rank);
};
#endif
