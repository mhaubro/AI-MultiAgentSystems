/*
 * RequestFreeSpaceTask.h
 *
 *  Created on: 17 May 2018
 *      Author: martin
 */

#ifndef SEARCHER_SRC_REQUESTFREESPACETASK_H_
#define SEARCHER_SRC_REQUESTFREESPACETASK_H_

#include "Task.h"
#include <utility>
#include "Agent.h"
#include "Box.h"
#include "Node.h"

class RequestFreeSpaceTask : public Task
{
public:
  std::pair<int, int> destination;
  int rank;

  std::vector<std::pair<int,int>> locations;//Locations to be freed

  RequestFreeSpaceTask(std::pair<int, int> loc, int rank);
  bool seemsCompleted(Agent * a, Node * n);
  bool isCompleted(Agent * a, Node * n);
  int h(Agent * a, Node * n);
};

#endif /* SEARCHER_SRC_REQUESTFREESPACETASK_H_ */
