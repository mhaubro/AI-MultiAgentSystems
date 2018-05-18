#include "Agent.h"
#include "Task.h"
#include "HandleGoalTask.h"
#include <functional>
#include <utility>
#include <boost/pool/object_pool.hpp>
#include "Entity.h"
#include "Strategy.h"
#include "CentralPlanner.h"
#include "RequestFreeSpaceTask.h"
#include <iostream>

//#define BACKUPLOOKAHEAD 5

std::list<Node *> Agent::search(Node * state){
	return a_star_search(state, this, this->task);
}

//Commits a search where all things are gone, and asks for the locations.
std::list<Node *> Agent::Nakedsearch(Node * state){
	Node nakedstate = *state;
	nakedstate.clearOtherAgents(this->chr);
	return a_star_search(state, this, this->task);
}

//Commits a search where all things are gone, and asks for the locations.
std::list<Node *> Agent::noBoxesOrAgents(Node * state, Box * box){
	Node nakedstate = *state;
	nakedstate.clearOtherAgentsAndBoxes(this->chr, box);
	return a_star_search(state, this, this->task);
}

Command * Agent::noPlan(Node * startstate){
	std::cerr<< "Doing first plan\n";
	//Short-cirrcuit. We have a task, which is not completed
	if (this->task != NULL && !this->task->seemsCompleted(this, startstate)){
		//Task wasn't completed, let's replan
		std::cerr << "Doing replanning with original task\n";
		//Do replanning
		//delete plan;
		std::list<Node *> searchResult = search(startstate);

		if (searchResult.empty()){
			//No agents, try path
			if (HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(this->task)){
				//				std::cerr << "Assigned task " << tmp->box->chr << " to agent " << this->chr << "\n";
				searchResult = Nakedsearch(startstate);

				//No boxes or agents, try path
				if (searchResult.empty()){
					searchResult = noBoxesOrAgents(startstate, tmp->box);
					if (searchResult.empty()){
						//We return the task to the centralplanner, and see if it has a helpjob
						cPlanner.removeRequestTask(t);
						//delete t;
						t = NULL;
						cPlanner.returnGoalTask((HandleGoalTask *)task);
						this->task = NULL;
						return &Command::EVERY[0];
					}
					//We have a path without agents and boxes, we proceed
				}
				//We have a path without agents, we proceed
			}

			std::cerr << "Wanting to add a request\n";
			plan = new Plan(searchResult, this->getLocation());
			t = new RequestFreeSpaceTask(plan->getLocations(), rank);
			//Do something
			cPlanner.addRequestFreeSpaceTask(t);
		}

		plan = new Plan(searchResult, this->getLocation());
		std::cerr << plan->toString() << "\n";
		Node::resetPool();
		return NULL;
	}
	//We don't have a task/have completed
	else if(!cPlanner.hasJob(this, startstate)){
		//Noone has requested anything.

		//Maybe we should improve our positioning, by moving away from other agents??
		return &Command::EVERY[0];

	} else {
		//Task was completed, there's more tasks for us.
		std::cerr <<"Task was: " << task << " Doing replanning\n" ;

		//Remove any requests for more space. If it's null, doesn't matter.
		cPlanner.removeRequestTask(t);
		//delete t;
		t = NULL;


		task = NULL;

		//Do replanning
		//delete plan;

		if (cPlanner.hasJob(this, startstate)){
			task = cPlanner.getJob(this, startstate);
			if (HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(this->task)){
				std::cerr << "Goal: "<< tmp->chr << " destination: " << tmp->destination.first <<"," << tmp->destination.second << " box: " << tmp->box->chr << "\n";
			}
			std::list<Node *> searchResult = search(startstate);
			std::cerr << "Received a task\n";
			if (searchResult.empty()){
				std::cerr << "Empty plan?\n";
				return &Command::EVERY[0];;
				//Do something
			}

			plan = new Plan(searchResult, this->getLocation());
			return NULL;
		}

		return &Command::EVERY[0];;
	}

}

Command * Agent::handleConflict(){
	if (t){//RequestFreeSpaceTask
		//cPlanner.removeTask(t);
		skipNextIte = 2;
	} else {
		if (plan != NULL && !(plan->locations.empty())){
			cPlanner.removeRequestTask(t);
			//delete t;
			t = new RequestFreeSpaceTask(plan->locations, rank);
		}
	}
	double prob = 0.3;
	if (((double)rand())/RAND_MAX + prob > 1)
		skipNextIte = 2;
	//std::cerr << "Conflict1!\n";
	//Do replanning next time
	plan->drain();
	return &Command::EVERY[0];
}

Command * Agent::getAction(Node * startstate, Node * tempstate){
	if (skipNextIte){
		skipNextIte--;
		return &Command::EVERY[0];
	}
	if (startstate->isGoalState(this->color))
	{
		std::cerr << "agent: " << chr << " goal\n";
		//NoOp
		return &Command::EVERY[0];
	}
	if (plan == NULL || plan->isEmpty()){
		if (Command * c = noPlan(startstate)){
			return c;
		}
	}
	//Find next step
	Command * c = plan->getStep();
	std::cerr << "Getting a step " << c->toString() << "\n";
	if (c == NULL){
		plan->drain();
		return &Command::EVERY[0];
	}
	//Find number
	int number = (int)(chr - '0');

	if (!startstate->checkState(number, c)){

		return handleConflict();
	}
	if (!tempstate->checkAndChangeState(number, c)){
		return handleConflict();
	}
	plan->popStep();
	return c;
}


Agent::Agent(char chr, int rank, std::pair<int, int> location, COLOR color):
																Entity(chr, location, color){
	this->task = nullptr;
	this->rank = rank;
	plan = NULL;
	t = NULL;

}

Agent::Agent(char chr, std::pair<int, int> location, COLOR color):
																Entity(chr, location, color)
{
	this->task = nullptr;
	this->rank = 0;
	plan = NULL;
	t = NULL;
}
//No color, for single agent levels
Agent::Agent(char chr, std::pair<int, int> location):
																Entity(chr, location, Entity::BLUE)
{
	this->task = nullptr;
	this->rank = 0;
	plan = NULL;
	t = NULL;
}

Agent::Agent(Agent * agt):
																Entity(agt->chr, agt->location, agt->color)
{
	this->task = agt->task;
	this->rank = agt->rank;
	this->plan = agt->plan;
	t = NULL;
}

int Agent::hashCode()
{
	int result = 17;
	result = 31 * result + (int) chr;
	result = 31 * result + rank;
	result = 31 * result + getX();
	result = 31 * result + getY();
	result = 31 * result + color;
	return result;
}

bool Agent::equals(const Agent * agent) const
{
	if (agent == this) return true;
	return agent->chr == chr && agent->rank == rank && agent->color == color && agent->location == location;
}
