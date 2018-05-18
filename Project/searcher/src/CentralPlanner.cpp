#include "CentralPlanner.h"
#include "HandleGoalTask.h"
#include <iostream>
#include <stack>
#include <list>
#include <algorithm>

CentralPlanner cPlanner;

CentralPlanner::CentralPlanner(){
	for(int i = 0; i < 8; i++){
		this->UnassignedTasks.push_back(std::stack<Task*>());
	}
	compatibleGoals = std::vector<std::vector<bool>>(Entity::NUMCOLS);
}

void CentralPlanner::preAnalyse(Node * n){
	getCompatibleGoals(n);
  getOrderOfAllGoals(n);
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

std::vector<Goal> CentralPlanner::getOrderOfAllGoals(Node * n)
{
  Node * tmp_state;
  std::vector<Goal> order;
  for(int i = 0; i < tmp_state->goals.size(); i++)
  {
    tmp_state = n;
    order.clear();
    order.emplace_back(tmp_state->goals[i]);
    for(int j = 0; j < tmp_state->goals.size(); j++)
    {
      if(i == j)
        continue;

      // See if this order works
      tmp_state = getOrderOfGoals(tmp_state, tmp_state->goals[i], tmp_state->goals[j]);

      // Try next box if it does not
      if(tmp_state == nullptr)
        continue;
      else
        order.emplace_back(tmp_state->goals[j]);
    }
    if(tmp_state != nullptr && tmp_state->isGoalState())
      return order;
  }
}

Node * CentralPlanner::getOrderOfGoals(Node * n, Goal g1, Goal g2)
{
  Node * new_state = nullptr;

  new_state = FindSolution(n, g1);

  if(new_state->isGoalState(g1))
  {
    new_state = FindSolution(new_state, g2);
    if(new_state->isGoalState(g2) && new_state->isGoalState(g1))
    {
      // std::cerr << "\n" << g1.chr << " and then " << g2.chr << "\n";
      // std::cerr << new_state->toString();
      return new_state;
    }
  }
  return nullptr;
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

Node * CentralPlanner::FindSolution(Node * n, Goal g)
{
  for(auto & b : n->boxes)
  {
    if(std::tolower(g.chr) == std::tolower(b.chr))
    {
      HandleGoalTask * t = new HandleGoalTask(&b, g.getLocation(), 0);
      // Find agent to solve task
      for(auto & a : n->agents)
      {
        if(a.color == t->box->color)
        {
          a.task = t;
          std::list<Node*> solution = a.search(n);
          a.task = nullptr; // Not sure if needed
          return solution.back();
        }
      }
    }
  }
  return nullptr;
}

Task * CentralPlanner::RequestTask(){
	return NULL;
}

Task * CentralPlanner::RequestHelp(){
	return NULL;
}
