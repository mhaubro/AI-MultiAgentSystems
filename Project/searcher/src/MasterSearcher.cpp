#include <string>
#include <list>
#include <iostream>
#include "Agent.h"
#include "MasterSearcher.h"
#include "Node.h"
//Returns a list of strings to be printed when printing the total plan.
namespace MasterSearcher
{
  bool checkConflicts()
  {
	   return false;
  }

  std::vector<std::string> getPlan(Node * initialState)
  {

    int agents = initialState->agents.size();
    std::cerr << "Agents: " << agents << "\n";
  	std::vector<std::list<Node *>> plans(agents);

  	int longestPlan = 0;
  	Node * state = initialState;

    // We need to do this multiple times
    CentralPlanner c = CentralPlanner();
    c.DetectTasks(state);
    c.AssignTasks(state);

    for (int i = 0; i < agents; i++)
    {
      plans[i] = state->agents[i].search(state);
      if (plans[i].size() > longestPlan)
        longestPlan = plans[i].size();
    }

    /*Creates a list of strings that is the actual plan*/
  	/*Keeps checking for conflicts*/
  	std::vector<std::string> plan = std::vector<std::string>();

  	bool isPlanning = true;
  	for (int i = 0; i < longestPlan; i++)
    {
  		std::string s = "[";
  		for (int j = 0; j < agents; j++)
      {
  			if (plans[j].size() == 0)
  				s += "NoOp";
  			/*Checks if we're planning or if there's been a conflict, and if the next move is okay*/
  			else if (state->checkAndChangeState(j, plans[j].front()->action) && isPlanning)
  				/*If the plans are okay, no conflict */
  				s += (plans[j].front()->action->toString());
        else
        {
  				isPlanning = false;
  				/*Do replanning*/
  				s += "NoOp";
  			}
  			if (j == agents-1)
  				s += ']';
  			else
  				s += ", ";

  			if (plans[j].size() > 0)
  				plans[j].pop_front();
  		}
  		plan.push_back(s);
  		if (state->isGoalState())
  			return plan;
  		if (!isPlanning)
  			break;
  	}
  	return plan;
  }
}
