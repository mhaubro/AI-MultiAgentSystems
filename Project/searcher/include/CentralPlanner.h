#ifndef CENTRALPLANNER_H
#define CENTRALPLANNER_H
#include "Box.h"
#include "Goal.h"
#include "Task.h"
#include "Agent.h"
#include <vector>
#include <stack>
#include "RequestFreeSpaceTask.h"
#include "HandleGoalTask.h"

class Task;
class HandleGoalTask;
class RequestFreeSpaceTask;

class CentralPlanner {
public:
  std::vector<HandleGoalTask*> UnassignedGoals;
  std::vector<RequestFreeSpaceTask *> freeSpaceTasks;

  //std::vector of goals, each containing a vector of size 8,
  std::vector<std::vector<bool>> compatibleGoals;

  CentralPlanner();
  bool isGoalCompatible(int goal, Entity::COLOR color);
  void preAnalyse(Node * n);

  void DetectTasks(Node * n);
  void AssignTasks(Node * n);
  void AssignTask(Agent * a, Node * state);
  bool TaskAvailable(Agent * a);

  bool hasJob(Agent * agent, Node * state);
  Task * getJob(Agent * agent, Node * state);
  void removeTask(Task * t);
  bool addRequestFreeSpaceTask(RequestFreeSpaceTask * h);
  bool returnGoalTask(HandleGoalTask * h);
  bool removeRequestTask(RequestFreeSpaceTask * h);



private:
  void getCompatibleGoals(Node * n);
};

extern CentralPlanner cPlanner;

#endif
