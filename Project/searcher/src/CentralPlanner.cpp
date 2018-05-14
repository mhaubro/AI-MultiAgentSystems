#include "CentralPlanner.h"
#include <iostream>
#include <stack>


CentralPlanner cPlanner;

CentralPlanner::CentralPlanner(){
	for(int i = 0; i < 8; i++){
		this->UnassignedTasks.push_back(std::stack<Task*>());
	}
	compatibleGoals = std::vector<std::vector<bool>>(Entity::NUMCOLS);
}

void CentralPlanner::preAnalyse(Node * n){
	getCompatibleGoals(n);
	//Should we check order of goals, if they are down a corridor?
	//We should
	//DetectTasks(n);
}

bool CentralPlanner::isGoalCompatible(int goal, Entity::COLOR color){
	return compatibleGoals[goal][color];
}

void CentralPlanner::getCompatibleGoals(Node * n){
	//Colors assumed to be the same as in entity. Enums are KNOWN to start at zero.
	compatibleGoals.resize(n->goals.size());
	for (int i = 0; i < n->goals.size(); i++){
		compatibleGoals[i] = std::vector<bool>(8);
		for (int j = 0; j < n->boxes.size(); j++){
			//Going through all boxes, storing if the color matches.
			if (tolower(n->boxes[j].chr) == n->goals[i].chr){
				compatibleGoals[i][ n->boxes[j].color] = true;
			}
		}
	}

}


void CentralPlanner::DetectTasks(Node * n)
{
	for(auto & g : n->goals)
	{
		Box * b = n->getBox(g.getX(), g.getY());
		//Goal is already done
		if (b != NULL && std::tolower(b->chr) == g.chr)
			continue;
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
