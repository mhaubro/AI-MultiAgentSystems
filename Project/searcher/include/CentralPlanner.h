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

  //std::vector of goals, each containing a vector of size 8,
  std::vector<std::vector<bool>> compatibleGoals;

  CentralPlanner();
  bool isGoalCompatible(int goal, Entity::COLOR color);
  void preAnalyse(Node * n);

  void DetectTasks(Node * n);
  void AssignTasks(Node * n);
  void AssignTask(Agent * a);
  bool TaskAvailable(Agent * a);
  Task * RequestTask();
  Task * RequestHelp();
  bool hasJob(Agent * agent);
  Task * getJob(Agent agent);


private:
  void getCompatibleGoals(Node * n);
};

extern CentralPlanner cPlanner;

#endif
