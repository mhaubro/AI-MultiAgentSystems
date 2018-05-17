#include "CentralPlanner.h"
#include <iostream>
#include <stack>
#include "HandleGoalTask.h"

CentralPlanner cPlanner;

//TODO
void CentralPlanner::removeTask(Task * t){

}


CentralPlanner::CentralPlanner(){
	UnassignedGoals = std::vector<HandleGoalTask *>();
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
bool CentralPlanner::hasJob(Agent * agent, Node * state){
	for (HandleGoalTask * h : UnassignedGoals){
		if (h->solvingColors[agent->color])
			return true;
	}

	for (RequestFreeSpaceTask * t : freeSpaceTasks){
		if (!t->seemsCompleted(agent, state))
			return true;
	}

	return false;
}

Task * CentralPlanner::getJob(Agent * agent, Node * state){

	//The agent will be the only one to get this task

	//For all boxes and goals, find the one with lowest h-value.
	for (int i = 0; i < UnassignedGoals.size(); i++){
		HandleGoalTask * h = UnassignedGoals[i];
		if (h->seemsCompleted(agent, state))
			continue;
		if (h->solvingColors[agent->color]){
			//Deletes the move box thing
			UnassignedGoals.erase(UnassignedGoals.begin()+i);
			//Finds most fitting box, by going through all setting and calculating h-val
			Box * bestBox;
			for (Box b : state->boxes){
				if (b.workInProgress)
					continue;
				double hval = 10000000000.0;
				h->box = &b;
				double boxh = h->h(agent, state);
				if (boxh > hval){
					hval = boxh;
					bestBox = &b;
				}
			}
			h->box = bestBox;
			h->box->workInProgress = true;
		}
			return h;

	}

	for (RequestFreeSpaceTask * t : freeSpaceTasks){
		if (!t->seemsCompleted(agent, state))
			return t;
	}


	return NULL;
}

void CentralPlanner::DetectTasks(Node * n)
{
	for(int i = 0; i < n->goals.size(); i++)
	{
		Goal g = n->goals[i];
		HandleGoalTask * t = new HandleGoalTask(g.getLocation(), 0, compatibleGoals[i]);
		UnassignedGoals.push_back(t);
	}
}

/*
void CentralPlanner::AssignTask(Agent * a, Node * state)
{
	if(cPlanner.UnassignedGoals[a->color].empty())
	{
		a->task = nullptr;
	}
	else
	{
		a->task = cPlanner.UnassignedGoals[a->color].top();
		cPlanner.UnassignedGoals[a->color].pop();
	}
}
*/

Task * CentralPlanner::RequestTask(){
	return NULL;
}

Task * CentralPlanner::RequestHelp(){
	return NULL;
}
