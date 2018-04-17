#ifndef TASK_H
#define TASK_H

#include "Agent.h"
#include "Box.h"

class Agent;

class Task
{
  public:
    Task();
    int manhattan(std::pair<int, int> loc1, std::pair<int, int> loc2);
    int h();
};

class MoveAgentTask : public Task
{
  public:
    Agent * agent;
    std::pair<int, int> destination;
    int rank;

    MoveAgentTask(Agent * agent, std::pair<int, int> loc, int rank);
    int h();
};

class MoveBoxTask : public Task
{
  public:
    Agent * agent;
    Box * box;
    std::pair<int, int> destination;
    int rank;

    MoveBoxTask(Agent * agent, Box * box, std::pair<int, int> loc, int rank);
    int h();
};
#endif
