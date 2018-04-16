#ifndef CENTRALPLANNER_H
#define CENTRALPLANNER_H
#include "Box.h"
#include "Goal.h"

#include <vector>

class CentralPlanner {
public:
  std::vector<Box *> UnassignedBoxes;
  std::vector<Goal *> UnassignedGoals;

  CentralPlanner(std::vector<Box *> boxes, std::vector<Goal *> goals);

  void AssignTasks();
  Task * RequestTask();
  Task * RequestHelp();
};


#endif
