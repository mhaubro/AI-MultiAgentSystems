#include "CentralPlanner.h"
#include "HandleGoalTask.h"
#include <iostream>
#include <stack>
#include <list>
#include <algorithm>
#include <ctime>
#include <cstdlib>

CentralPlanner::CentralPlanner(int region){
	this->region = region;
	UnassignedGoals = std::vector<HandleGoalTask *>();
	compatibleGoals = std::vector<std::vector<bool>>(Entity::NUMCOLS);
}

void CentralPlanner::preAnalyse(Node * n){
	getCompatibleGoals(n);
	DetectTasks(n);
	std::cerr << "Returning falseBox\n";
//setPredecessors(n);
}

bool CentralPlanner::isGoalCompatible(int goal, Entity::COLOR color){
	return compatibleGoals[goal][color];
}

void CentralPlanner::getCompatibleGoals(Node * n){
	//This function should preferably just calculate all goals, as we don't care about overhead, and accessing the
	//Goals number in here seems to be pretty convenient.
	//Colors assumed to be the same as in entity. Enums are KNOWN to start at zero.
	compatibleGoals.resize(n->goals.size());
	for (int i = 0; i < n->goals.size(); i++){
		compatibleGoals[i] = std::vector<bool>(8);
		for (int j = 0; j < n->boxes.size(); j++){
			//Going through all boxes, storing if the color matches.
			if (tolower(n->boxes[j].getChar()) == n->goals[i].getChar()){
				compatibleGoals[i][n->boxes[j].getColor()] = true;
			}
		}
	}
}

void CentralPlanner::setPredecessors(Node * n)
{
	for(int i = 0; i < n->goals.size(); i++)
	{
		for(int j = 0; j < n->goals.size(); j++)
		{
			if(i == j)
        continue;

      // There is a dependency
      if(getOrderOfGoals(n, n->goals[i], n->goals[j]))
        setPredecessor(n->goals[i].getChar(), n->goals[j].getChar());
		}
	}
}

void CentralPlanner::setPredecessor(char g1, char g2)
{
  // Find its task and set it
  for(int k = 0; k < UnassignedGoals.size(); k++)
  {
    // Only need to look at the task fitting the goal
    if(UnassignedGoals[k]->chr != g1)
      continue;

    for(int l = 0; l < UnassignedGoals.size(); l++)
    {
      // Only need to look at the task fitting the goal
      if(k == l || UnassignedGoals[l]->chr != g2)
        continue;

      UnassignedGoals[l]->predecessors.push_back(UnassignedGoals[k]);
    }
  }
}

// Checks if g1 needs to be solved before g2
bool CentralPlanner::getOrderOfGoals(Node * n, Goal g1, Goal g2)
{
	Node * state = nullptr;
  std::vector<Location> locations = n->recordAgentLocations();

  state = FindSolution(n, g1);

  if(state == nullptr)
    return false;

  state = FindSolution(n, g2);

  // Can g2 be solved?
  if(state->isGoalState(g2))
  {
    state->resetAgent(locations);
    state = FindSolution(state, g1);

    // If we cannot solve g1 after having solved g2,
    // then that means g1 needs to be solved first
    if(state == nullptr)
      return true;
  }
  return false;
}


int myrandom (int i) { return std::rand()%i;}

bool CentralPlanner::hasJob(Agent * agent, Node * state){
	std::srand ( unsigned ( std::time(0) ) );
	for (HandleGoalTask * h : UnassignedGoals){
		if (h->solvingColors[agent->getColor()] && !h->seemsCompleted(agent, state))
			return true;
	}

	std::random_shuffle ( freeSpaceTasks.begin(), freeSpaceTasks.end(), myrandom);

	for (RequestFreeSpaceTask * t : freeSpaceTasks){
		if (!t->seemsCompleted(agent, state))
			return true;
	}

	return false;
}

Task * CentralPlanner::getJob(Agent * agent, Node * state){
	//std::cerr << "Finding a job for someone\n";
	//The agent will be the only one to get this task
	//For all boxes and goals, find the one with lowest h-value.
	for (int i = 0; i < UnassignedGoals.size(); i++){
		//if(!UnassignedGoals[i]->predecessorsComplete(agent, state))
      //continue;

		HandleGoalTask * h = UnassignedGoals[i];
		//std::cerr << "Trying with goal " << state->getGoal(h->destination.first, h->destination.second)->chr << "\n";

		if (h->seemsCompleted(agent, state))
			continue;
		if (h->solvingColors[agent->getColor()]){
			//std::cerr << "There's a solvable goal\n";
			//Deletes the move box thing
			//Finds most fitting box, by going through all setting and calculating h-val
			Box * bestBox;
			double hval = 10000000000.0;
			for (auto &b : state->boxes){
				if ((tolower(b.getChar()) != h->chr) || b.workInProgress)
					continue;
				h->box = &b;
				double boxh = h->h(agent, state);
				if (boxh < hval){
					//std::cerr << "Good box found "<< b.chr << "\n";
					hval = boxh;
					bestBox = &b;
				}
			}
			h->box = bestBox;
			h->box->workInProgress = true;
			//std::cerr << "Task assigned with box " << h->box->chr << "\n";
		} else {
			continue;
		}
		UnassignedGoals.erase(UnassignedGoals.begin()+i);
		//throw "BUG";
		return h;
	}
	//std::cerr << "Agent " << agent->chr<<  "has nothing to do\n";

	for (RequestFreeSpaceTask * t : freeSpaceTasks){
		//std::cerr << "Checking for requests \n";
		if (!t->seemsCompleted(agent, state)){
			return t;
		}
	}

	return NULL;
}

bool CentralPlanner::addRequestFreeSpaceTask(RequestFreeSpaceTask * h){
	//std::cerr << "Adding a request\n";
	if (h == NULL)
		return false;
	freeSpaceTasks.push_back(h);
	return true;
}

//Returns a goal task from an agent to the planner
bool CentralPlanner::returnGoalTask(HandleGoalTask * h){
	if (h != NULL){
		this->UnassignedGoals.push_back(h);
		return true;
	}
	return false;
}


bool CentralPlanner::removeRequestTask(RequestFreeSpaceTask * h){
	if (h == NULL)
		return true;
	for (int i = 0; i < freeSpaceTasks.size(); i++){
		if (freeSpaceTasks[i] == h){//Yes, a pointer match
			freeSpaceTasks.erase(freeSpaceTasks.begin()+i);
			return true;
		}
	}
	return false;
}

void CentralPlanner::DetectTasks(Node * n)
{
	for(int i = 0; i < n->goals.size(); i++)
	{
		Goal * g = &n->goals[i];
		if (g->getRegion() != region)
			continue;
		HandleGoalTask * t = new HandleGoalTask(g->getLocation(), 100, compatibleGoals[i], g->getChar());
		UnassignedGoals.push_back(t);
	}
	//throw "Crash\n"
}

bool CentralPlanner::stillActiveRequest(RequestFreeSpaceTask * h){
	if (h == NULL)
		return false;
	for (int i = 0; i < freeSpaceTasks.size(); i++){
		if (freeSpaceTasks[i] == h){//Yes, a pointer match
			return true;
		}
	}
	return false;
}


Node * CentralPlanner::FindSolution(Node * n, Goal g)
{
	for(auto & b : n->boxes)
	{
		if(std::tolower(g.getChar()) == std::tolower(b.getChar()))
		{
			HandleGoalTask * t = new HandleGoalTask(g.getLocation(), 0, &b);
			// Find agent to solve task
			for(auto & a : n->agents)
			{
				if(a.getColor() == t->box->getColor())
				{
					a.task = t;
					std::list<Node*> solution = a.search(n);
					a.task = nullptr;
					return solution.back();
				}
			}
		}
	}
	return nullptr;
}

RequestFreeSpaceTask * CentralPlanner::getHelpJob(Agent * agent, Node * state){
	for (RequestFreeSpaceTask * t : freeSpaceTasks){
		//std::cerr << "Checking for requests \n";
		if (!t->seemsCompleted(agent, state)){
			return t;
		}
	}
	return NULL;
}

bool CentralPlanner::hasHelpJob(Agent * agent, Node * state){
	return (NULL != getHelpJob(agent, state));
}

