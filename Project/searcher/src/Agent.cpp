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

Command * Agent::noPlan(Node * startstate){
	//Short-cirrcuit. We have a task, which is not completed
	if (this->task != NULL && !this->task->isCompleted(this, startstate)){
		//Task wasn't completed, let's replan
		std::cerr << "Doing replanning with original task\n";
		//Do replanning
		delete plan;
		//Task* tmp = reinterpret_cast<Task*>(this->task);
		//std::cerr << "Assigned task " << tmp->box->chr << " to agent " << this->chr << "\n";
		std::list<Node *> searchResult = search(startstate);

		if (searchResult.empty()){
			searchResult = Nakedsearch(startstate);
			plan = new Plan(searchResult, this->getLocation());
			RequestFreeSpaceTask * t = new RequestFreeSpaceTask(plan->getLocations(), rank);
			//Do something
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
		//Do replanning
		delete plan;
		if (cPlanner.hasJob(this, startstate)){
			cPlanner.getJob(this, startstate);
		}
		std::list<Node *> searchResult = search(startstate);
		if (searchResult.empty()){
			//Do something
		}
		plan = new Plan(searchResult, this->getLocation());
		std::cerr << plan->toString() << "\n";
		return NULL;
	}

}

Command * Agent::handleConflict(){
	if (t){//RequestFreeSpaceTask
		cPlanner.removeTask(t);
	}
	double prob = 0.3;
	if (((double)rand())/RAND_MAX + prob > 1)
		skipNextIte = true;
	//std::cerr << "Conflict1!\n";
	//Do replanning next time
	plan->drain();
	return &Command::EVERY[0];
}

Command * Agent::getAction(Node * startstate, Node * tempstate){
	if (skipNextIte){
		skipNextIte = false;
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
	return c;
}


Agent::Agent(char chr, int rank, std::pair<int, int> location, COLOR color):
				Entity(chr, location, color){
	this->task = nullptr;
	this->rank = rank;
	plan = NULL;
}

Agent::Agent(char chr, std::pair<int, int> location, COLOR color):
				Entity(chr, location, color)
{
	this->task = nullptr;
	this->rank = 0;
	plan = NULL;
}
//No color, for single agent levels
Agent::Agent(char chr, std::pair<int, int> location):
				Entity(chr, location, Entity::BLUE)
{
	this->task = nullptr;
	this->rank = 0;
	plan = NULL;
}

Agent::Agent(Agent * agt):
				Entity(agt->chr, agt->location, agt->color)
{
	this->task = agt->task;
	this->rank = agt->rank;
	this->plan = agt->plan;
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
