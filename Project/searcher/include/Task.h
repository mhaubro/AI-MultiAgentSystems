#ifndef TASK_H
#define TASK_H

#include <utility>

class Agent;
class Node;
class Box;

class Task
{
public:
	//Dynamic casting should make the above superfluous
//  enum class Type {
//    MoveAgentTask, MoveBoxTask
//  };
  int rank;
//  Type type;

  virtual bool seemsCompleted(Agent * a, Node * n);
  virtual bool isCompleted(Agent * a, Node * n);
  virtual int h(Agent * a, Node * n);
  int manhattan(std::pair<int, int> loc1, std::pair<int, int> loc2);
};


#include "Agent.h"
#include "Box.h"
#include "Node.h"

#endif
