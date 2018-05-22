#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include "Agent.h"
#include "MasterSearcher.h"
#include "Node.h"
//Returns a list of strings to be printed when printing the total plan.
namespace MasterSearcher
{
void getPlan(Node * initialState)
				  {
	int agents = initialState->agents.size();

	//Get number of regions
	int regions = 0;
	for (int i = 0; i < agents; i++){
		int reg = initialState->agents[i].getRegion();
		if (reg > regions)
			regions = reg;
	}
	regions++;

	//Init centralPlanners
	std::vector<CentralPlanner> planners = std::vector<CentralPlanner>();
	for (int i = 0; i < regions; i++){
		//Create a planner with a region
		planners.emplace_back(i);
		planners[i].preAnalyse(initialState);
	}

	//Give each agent a pointer to its planner
	for (int i = 0; i < agents; i++){
		Agent * agent = &initialState->agents[i];
		agent->setMyPlanner(&planners[agent->getRegion()]);
	}

	Node state = *initialState;
	Node tempstate = state;


	while (true)
	{
		std::string s = "[";
		for (int i = 0; i < agents; i++)
		{
			//s += "NoOp";
			/*Checks if we're planning or if there's been a conflict, and if the next move is okay*/
			s += (tempstate.agents[i].getAction(&state, &tempstate)->toString());
			if (i == agents-1)
				s += ']';
			else
				s += ", ";
		}

		state = tempstate;
		//std::cerr << s << "\n";// << tempstate.toString() << "\n";
		printStep(s);
		if (state.isGoalState())
			return;
	}
	return;
				  }


void printStep(std::string s){
	std::stringstream ss;

	std::cout << s;
	std::cout << "\n";
	std::string response;
	////std::cerr << "Printing solution3";
	std::getline(std::cin, response);
	////std::cerr << "Printing solution4";
	if (response.find(std::string("false")) != std::string::npos) {
		ss << "Server responsed with "<< response <<"%s to the inapplicable action: "<< s <<"\n";
		//std::cerr << ss.str();
		ss.str("");
		//Maybe we should actually be able to print the state. However, sending
		//A plan assumes that the thing actually works, and we will never
		//Replan out here.
		ss << s << " was attempted in a state\n";
		//std::cerr << ss.str();
		throw ("Command Error, printing step\n");
	}
}
}
