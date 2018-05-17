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

class Node;
class Agent;

class RequestFreeSpaceTask : public Task
{
public:
  int rank;

  std::list<std::pair<int,int>> locations;//Locations to be freed

  RequestFreeSpaceTask(std::list<std::pair<int, int>> locations, int rank);
  bool seemsCompleted(Agent * a, Node * n);
  bool isCompleted(Agent * a, Node * n);
  int h(Agent * a, Node * n);

  void popLocation();//Removes a location, if the agent is able to progress.

};

#include "Agent.h"
#include "Node.h"


#endif /* SEARCHER_SRC_REQUESTFREESPACETASK_H_ */
