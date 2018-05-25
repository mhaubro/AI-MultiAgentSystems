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
  Node * getOrderOfGoals(Node * n, Goal g1, Goal g2);
  std::vector<Goal> getOrderOfAllGoals(Node * n);
  void removeTask(Task * t);
  void setPredecessors(std::vector<Goal> order, std::vector<HandleGoalTask *> tasks);
  bool addRequestFreeSpaceTask(RequestFreeSpaceTask * h);
  bool returnGoalTask(HandleGoalTask * h);
  bool removeRequestTask(RequestFreeSpaceTask * h);

  RequestFreeSpaceTask * getHelpJob(Agent * agent, Node * state);
  bool hasHelpJob(Agent * agent, Node * state);

  int region;


private:
  void getCompatibleGoals(Node * n);
};

extern CentralPlanner cPlanner;

#endif
