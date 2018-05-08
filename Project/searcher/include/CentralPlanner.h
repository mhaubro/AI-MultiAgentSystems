#ifndef CENTRALPLANNER_H
#define CENTRALPLANNER_H
#include "Box.h"
#include "Goal.h"

#include <vector>

class CentralPlanner {
public:
  std::vector<Task*> UnassignedTasks;

  CentralPlanner();

  void DetectTasks(Node * n);
  void AssignTasks(Node * n);
  void AssignTask(Agent * a);
  bool TaskAvailable(Agent * a);
  Task * RequestTask();
  Task * RequestHelp();
};


#endif
