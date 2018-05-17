#include "CentralPlanner.h"
#include <iostream>
#include <stack>
#include "HandleGoalTask.h"

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


Task * CentralPlanner::RequestTask(){
	return NULL;
}

Task * CentralPlanner::RequestHelp(){
	return NULL;
}
