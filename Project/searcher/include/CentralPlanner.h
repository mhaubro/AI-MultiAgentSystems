#ifndef CENTRALPLANNER_H
#define CENTRALPLANNER_H
#include "Box.h"
#include "Goal.h"
#include "Task.h"
#include "Agent.h"
#include "RequestFreeSpaceTask.h"
#include "HandleGoalTask.h"
#include <vector>
#include <stack>
#include <list>
#include <algorithm>

class Task;
class Goal;
class HandleGoalTask;
class RequestFreeSpaceTask;

class CentralPlanner {
public:
  std::vector<HandleGoalTask*> UnassignedGoals;
  std::vector<RequestFreeSpaceTask *> freeSpaceTasks;
  std::vector<Goal> order;

  //std::vector of goals, each containing a vector of size 8,
  std::vector<std::vector<bool>> compatibleGoals;

  CentralPlanner(int region);
  bool isGoalCompatible(int goal, Entity::COLOR color);
  void preAnalyse(Node * n);

  bool stillActiveRequest(RequestFreeSpaceTask * h);

  void DetectTasks(Node * n);
  void AssignTasks(Node * n);
  void AssignTask(Agent * a, Node * state);
  bool TaskAvailable(Agent * a);

  bool hasJob(Agent * agent, Node * state);
  Task * getJob(Agent * agent, Node * state);
  Node * FindSolution(Node * n, Goal g);
  bool getOrderOfGoals(Node * n, Goal g1, Goal g2);
  void removeTask(Task * t);
  void setPredecessors(Node * n);
  void setPredecessor(char g1, char g2);
  bool addRequestFreeSpaceTask(RequestFreeSpaceTask * h);
  bool returnGoalTask(HandleGoalTask * h);
  bool removeRequestTask(RequestFreeSpaceTask * h);
  int region;


private:
  void getCompatibleGoals(Node * n);
};

extern CentralPlanner cPlanner;

#endif
