#ifndef TASK_H
#define TASK_H

#include "Agent.h"
#include "Box.h"
#include "Node.h"

class Agent;
class Node;

class Task
{
public:
  int rank;
  virtual bool isCompleted(Agent * a, Node * n);
};

class MoveAgentTask : public Task
{
  public:
    std::pair<int, int> destination;
    int rank;

    MoveAgentTask(std::pair<int, int> loc, int rank);
    bool isCompleted(Agent * a, Node * n);
};

class MoveBoxTask : public Task
{
  public:
    Box * box;
    std::pair<int, int> destination;
    int rank;

    MoveBoxTask(Box * box, std::pair<int, int> loc, int rank);
    bool isCompleted(Agent * a, Node * n);
};
#endif
