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
std::vector<std::string> getPlan(Node * initialState)
		  {
	int agents = initialState->agents.size();
	std::vector<std::list<Node *>> plans(agents);

	int longestPlan = 0;
	Node state = *initialState;
	Node tempstate = state;

	// We need to do this multiple times
	cPlanner = CentralPlanner();
	cPlanner.DetectTasks(&state);
	/*Creates a list of strings that is the actual plan*/
	/*Keeps checking for conflicts*/
	std::vector<std::string> plan = std::vector<std::string>();
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
		plan.push_back(s);
		std::cerr << s << "\n";
		printStep(s);
		if (state.isGoalState())
			return plan;
	}
	return plan;
		  }


void printStep(std::string s){
	std::stringstream ss;

	std::cout << s;
	std::cout << "\n";
	std::string response;
	//std::cerr << "Printing solution3";
	std::getline(std::cin, response);
	//std::cerr << "Printing solution4";
	if (response.find(std::string("false")) != std::string::npos) {
		ss << "Server responsed with "<< response <<"%s to the inapplicable action: "<< s <<"\n";
		std::cerr << ss.str();
		ss.str("");
		//Maybe we should actually be able to print the state. However, sending
		//A plan assumes that the thing actually works, and we will never
		//Replan out here.
		ss << s << " was attempted in a state\n";
		std::cerr << ss.str();
		throw ("Command Error, printing step\n");
	}
}
}
