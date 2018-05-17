#include "CentralPlanner.h"
#include "HandleGoalTask.h"
#include <iostream>
#include <stack>
#include <list>

CentralPlanner cPlanner;

CentralPlanner::CentralPlanner(){
	for(int i = 0; i < 8; i++){
		this->UnassignedTasks.push_back(std::stack<Task*>());
	}
	compatibleGoals = std::vector<std::vector<bool>>(Entity::NUMCOLS);
}

void CentralPlanner::preAnalyse(Node * n){
	getCompatibleGoals(n);
  getOrderOfGoals(n, n->goals[0], n->goals[3]);
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

void CentralPlanner::getOrderOfGoals(Node * n, Goal g1, Goal g2)
{
  std::cerr << "\n\n GET ORDER OF GOALS \n\n";
  Node * new_state = nullptr;

  new_state = FindSolution(n, g1);

  if(new_state->isGoalState(g1))
  {
    new_state = FindSolution(new_state, g2);
    if(new_state->isGoalState(g2))
    {
      std::cerr << "\n" << g1.chr << " and then " << g2.chr << "\n";
      std::cerr << new_state->toString();
    }
  }
  else
  {
    new_state = FindSolution(n, g2);

    if(new_state->isGoalState(g2))
    {
      new_state = FindSolution(new_state, g1);
      if(new_state->isGoalState(g1))
      {
        std::cerr << "\n" << g2.chr << " and then " << g1.chr << "\n";
        std::cerr << new_state->toString();
      }
    }
  }
}

//Todo
bool CentralPlanner::hasJob(Agent * agent){
	return cPlanner.UnassignedTasks[agent->color].size() != 0;
}

Task * CentralPlanner::getJob(Agent agent){
	return NULL;
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
				HandleGoalTask * t = new HandleGoalTask(&b, g.getLocation(), 0);
				this->UnassignedTasks[b.color].push(t);
			}
		}
	}
}

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
