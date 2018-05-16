#include "Agent.h"
#include "Task.h"
#include <functional>
#include <utility>
#include <boost/pool/object_pool.hpp>
#include "Entity.h"
#include "Strategy.h"
#include "CentralPlanner.h"
#include <iostream>
//boost::object_pool<Agent> Agent::pool;

std::list<Node *> Agent::search(Node * state){
	return a_star_search(state, this, this->task);
}

Command * Agent::noPlan(Node * startstate){
	//Short-cirrcuit. We have a task, which is not completed
	if (this->task != NULL && !this->task->isCompleted(this, startstate)){
		//Task wasn't completed, let's replan
		//std::cerr << "Doing replanning with original task\n";
		//Do replanning
		delete plan;
		MoveBoxTask* tmp = reinterpret_cast<MoveBoxTask*>(this->task);
		std::cerr << "Assigned task " << tmp->box->chr << " to agent " << this->chr << "\n";
		std::list<Node *> searchResult = search(startstate);
		if (searchResult.empty()){
			//It wasn't possible to compute a way to solve this
		}
		plan = new Plan(searchResult, getLocation());
		//std::cerr << plan->toString() << "\n";
		Node::resetPool();
	}
	//We don't have a task/have completed
	else if(!cPlanner.hasJob(this)){
		//We should listen in, see if other agents request something
		//std::cerr << "No more plans\n";
		return &Command::EVERY[0];
	} else {
		//Task was completed, there's more tasks for us.
		//std::cerr <<"Task was: " << task << " Doing replanning\n" ;
		//Do replanning
		delete plan;
		cPlanner.AssignTask(this);
		//Grab a task
		MoveBoxTask* tmp = reinterpret_cast<MoveBoxTask*>(this->task);
		std::cerr << "Assigned task " << tmp->box->chr << " to agent " << this->chr << "\n";
		std::list<Node *> searchResult = search(startstate);
		if (searchResult.empty()){
			//It wasn't possible to compute a way to solve this
		}
		plan = new Plan(searchResult, getLocation());
		//std::cerr << plan->toString() << "\n";
		Node::resetPool();
	}
	return NULL;
}

void Agent::handleConflict(){
	//Currently working on a task
	if (workingOnTask){
		skipNextIte = 2;

		//Check my next location

		return;
	}

	double prob = 0.3;
	if (((double)rand())/RAND_MAX + prob > 1)
		skipNextIte = 2;
	//std::cerr << "Conflict1!\n";
	//Do replanning next time
	//plan->drain();

}

Command * Agent::getAction(Node * startstate, Node * tempstate){
	Command * c = NULL;

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
		if (c = noPlan(startstate)){//Ugly, but fine
			return c;
		}
	}
	//Find next step
	c = plan->getStep();
	//Sanity check
	if (c == NULL){
		plan->drain();
		return &Command::EVERY[0];
	}
	//Find number
	int number = (int)(chr - '0');


	if (!startstate->checkState(number, c)){
		handleConflict();
		return &Command::EVERY[0];
	}
	if (!tempstate->checkAndChangeState(number, c)){
		handleConflict();
		return &Command::EVERY[0];
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
