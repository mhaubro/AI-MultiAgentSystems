/*
 * HandleGoalTask.h
 *
 *  Created on: 17 May 2018
 *      Author: martin
 */

#ifndef SEARCHER_SRC_HANDLEGOALTASK_H_
#define SEARCHER_SRC_HANDLEGOALTASK_H_

#include <utility>
#include <vector>
#include "Task.h"

class Box;
class Agent;
class Node;

class HandleGoalTask : public Task
{
public:
  std::vector<bool> solvingColors;
  HandleGoalTask * predecessors;//Goal that must be completed before this.

  Box * box;
  std::pair<int, int> destination;
  int rank;

  HandleGoalTask(std::pair<int, int> loc, int rank, std::vector<bool> solvingColors);
  bool seemsCompleted(Agent * a, Node * n);
  bool isCompleted(Agent * a, Node * n);
  int h(Agent * a, Node * n);
};

#include "Agent.h"
#include "Box.h"
#include "Node.h"


#endif /* SEARCHER_SRC_HANDLEGOALTASK_H_ */
