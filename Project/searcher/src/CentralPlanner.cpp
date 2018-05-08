#include "CentralPlanner.h"
#include <iostream>

CentralPlanner::CentralPlanner(){

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
        this->UnassignedTasks.push_back(t);
      }
    }
  }
}

void CentralPlanner::AssignTasks(Node * n)
{
  int s = this->UnassignedTasks.size();
  for(int i = 0; i < s; i++)
  {
    //MoveBoxTask* tmp = reinterpret_cast<MoveBoxTask*>(this->UnassignedTasks[i]);
    if(this->UnassignedTasks[i]->type == Task::Type::MoveBoxTask)
    {
      MoveBoxTask* tmp = reinterpret_cast<MoveBoxTask*>(this->UnassignedTasks[i]);
      for(auto & a : n->agents)
      {
        if(tmp->box->color == a.color)
        {
          a.task = this->UnassignedTasks[i];
          this->UnassignedTasks.erase(this->UnassignedTasks.begin());
        }
      }
    }
  }
}

Task * CentralPlanner::RequestTask(){

}

Task * CentralPlanner::RequestHelp(){

}
