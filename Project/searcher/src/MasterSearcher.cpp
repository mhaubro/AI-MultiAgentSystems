#include <string>
#include <list>
#include <iostream>
#include "Agent.h"
#include "MasterSearcher.h"
#include "Node.h"
//Returns a list of strings to be printed when printing the total plan.
namespace MasterSearcher
{



std::vector<std::string> getPlan(Node * initialState){

	int agents = initialState->agents.size();
	std::cerr << "Agents: " << agents << "\n";
	std::vector<std::list<Node *>> plans(agents);

	int longestPlan = 0;
	Node * state = initialState;
	Node * tempstate;

	// We need to do this multiple times
	CentralPlanner c = CentralPlanner();
	c.DetectTasks(state);
	c.AssignTasks(state);

	/*Creates a list of strings that is the actual plan*/
	/*Keeps checking for conflicts*/
	std::vector<std::string> plan = std::vector<std::string>();

	while (true){
		std::string s = "[";
		for (int i = 0; i < agents; i++)
		{
			//s += "NoOp";
			/*Checks if we're planning or if there's been a conflict, and if the next move is okay*/
			s += (tempstate->agents[i].getAction(state, tempstate)->toString());
			if (i == agents-1)
				s += ']';
			else
				s += ", ";
		}
		plan.push_back(s);
		if (state->isGoalState())
			return plan;

	}
	return plan;
}

}
