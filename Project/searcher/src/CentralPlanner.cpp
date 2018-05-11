#include "CentralPlanner.h"
#include <iostream>
#include <stack>


CentralPlanner cPlanner;

CentralPlanner::CentralPlanner(){
  for(int i = 0; i < 8; i++){
    this->UnassignedTasks.push_back(std::stack<Task*>());
  }
}

void CentralPlanner::DetectTasks(Node * n)
{
  for(auto & g : n->goals)
  {
    for(auto & b : n->boxes)
    {
      if(std::tolower(g.chr) == std::tolower(b.chr))
      {
        // Set rank?
        MoveBoxTask * t = new MoveBoxTask(&b, g.getLocation(), 0);
        this->UnassignedTasks[b.color].push(t);
      }
    }
  }
}
/*
void CentralPlanner::AssignTasks(Node * n)
{
  int s = this->UnassignedTasks.size();
  for(int i = 0; i < s; i++)
  {
    if(this->UnassignedTasks[i]->type == Task::Type::MoveBoxTask)
    {
      MoveBoxTask* tmp = reinterpret_cast<MoveBoxTask*>(this->UnassignedTasks[i]);
      for(auto & a : n->agents)
      {
        if(a.task != nullptr)
          continue;

        if(tmp->box->color == a.color)
        {
          a.task = this->UnassignedTasks[i];
          this->UnassignedTasks.erase(this->UnassignedTasks.begin());
        }
      }
    }
  }
}
*/
void CentralPlanner::AssignTask(Agent * a)
{
  if(cPlanner.UnassignedTasks[a->color].empty())
  {
    a->task = nullptr;
  }
  else
  {
    a->task = cPlanner.UnassignedTasks[a->color].top();
    cPlanner.UnassignedTasks[a->color].pop();
  }
}
/*
bool CentralPlanner::TaskAvailable(Agent * a)
{
  int s = this->UnassignedTasks.size();
  for(int i = 0; i < s; i++)
  {
    if(this->UnassignedTasks[i]->type == Task::Type::MoveBoxTask)
    {
      MoveBoxTask* tmp = reinterpret_cast<MoveBoxTask*>(this->UnassignedTasks[i]);
      if(tmp->box->color == a->color)
        return true;
    }
  }
  return false;
}
*/
Task * CentralPlanner::RequestTask(){

}

Task * CentralPlanner::RequestHelp(){

}
