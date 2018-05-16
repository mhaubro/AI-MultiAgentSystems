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
  enum class Type {
    MoveAgentTask, MoveBoxTask
  };
  int rank;
  Type type;

  virtual bool seemsCompleted(Agent * a, Node * n);
  virtual bool isCompleted(Agent * a, Node * n);
  virtual int h(Agent * a, Node * n);
  int manhattan(std::pair<int, int> loc1, std::pair<int, int> loc2);
};

/*Handle a goal*/
class handleGoalTask : public Task
{
public:
  handleGoalTask * predecessors;//Goal that must be completed before this.

  Box * box;
  std::pair<int, int> destination;
  int rank;

  handleGoalTask(Box * box, std::pair<int, int> loc, int rank);
  bool seemsCompleted(Agent * a, Node * n);
  bool isCompleted(Agent * a, Node * n);
  int h(Agent * a, Node * n);
};


/*Move agent out of the way*/
class MoveAgentTask : public Task
{
public:
  std::pair<int, int> destination;
  int rank;

  MoveAgentTask(std::pair<int, int> loc, int rank);
  bool seemsCompleted(Agent * a, Node * n);
  bool isCompleted(Agent * a, Node * n);
  int h(Agent * a, Node * n);
};

/*Moves box to release agent*/
class MoveBoxTask : public Task
{
public:
  Box * box;
  std::pair<int, int> destination;
  int rank;

  MoveBoxTask(Box * box, std::pair<int, int> loc, int rank);
  bool isCompleted(Agent * a, Node * n);
  bool seemsCompleted(Agent * a, Node * n);
  int h(Agent * a, Node * n);
};
#endif
