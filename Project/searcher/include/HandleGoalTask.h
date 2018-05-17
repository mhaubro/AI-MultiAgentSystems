/*
 * HandleGoalTask.h
 *
 *  Created on: 17 May 2018
 *      Author: martin
 */

#ifndef SEARCHER_SRC_HANDLEGOALTASK_H_
#define SEARCHER_SRC_HANDLEGOALTASK_H_

#include "Task.h"
#include <utility>
#include "Agent.h"
#include "Box.h"
#include "Node.h"


class HandleGoalTask : public Task
{
public:
  HandleGoalTask * predecessors;//Goal that must be completed before this.

  Box * box;
  std::pair<int, int> destination;
  int rank;

  HandleGoalTask(Box * box, std::pair<int, int> loc, int rank);
  bool seemsCompleted(Agent * a, Node * n);
  bool isCompleted(Agent * a, Node * n);
  int h(Agent * a, Node * n);
};

#endif /* SEARCHER_SRC_HANDLEGOALTASK_H_ */
