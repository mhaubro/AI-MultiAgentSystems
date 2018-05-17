#include "Task.h"
#include "Agent.h"
#include "Box.h"
#include <iostream>

bool Task::isCompleted(Agent * a, Node * n)
{
  return false;
}
bool Task::seemsCompleted(Agent * a, Node * n)
{
  return false;
}

int Task::h(Agent * a, Node * n)
{
  return 0;
}

int Task::manhattan(std::pair<int, int> loc1, std::pair<int, int> loc2)
{
  return abs(std::get<0>(loc2) - std::get<0>(loc1)) + abs(std::get<1>(loc2) - std::get<1>(loc1));
}

