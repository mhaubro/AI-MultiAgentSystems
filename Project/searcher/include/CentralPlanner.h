#ifndef CENTRALPLANNER_H
#define CENTRALPLANNER_H
#include "Box.h"
#include "Goal.h"
#include "Task.h"
#include <vector>
#include <stack>

class Task;

class CentralPlanner {
public:
  std::vector<std::stack<Task*>> UnassignedTasks;

  CentralPlanner();

  void DetectTasks(Node * n);
  void AssignTasks(Node * n);
  void AssignTask(Agent * a);
  bool TaskAvailable(Agent * a);
  Task * RequestTask();
  Task * RequestHelp();
};

extern CentralPlanner cPlanner;

#endif
