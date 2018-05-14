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

Command * Agent::getAction(Node * startstate, Node * tempstate){
	if (startstate->isGoalState(this->color))
	{
		std::cerr << "agent: " << chr << " goal\n";
		//NoOp
		return &Command::EVERY[0];
	}
	if (plan == NULL || plan->actions.size()==0){
		//Short-cirrcuit. We have a task, which is not completed
		if (this->task != NULL && !this->task->isCompleted(this, startstate)){
			//Task wasn't completed, let's replan
			std::cerr << "Doing replanning\n";
			//Do replanning
			delete plan;
			MoveBoxTask* tmp = reinterpret_cast<MoveBoxTask*>(this->task);
			std::cerr << "Assigned task " << tmp->box->chr << " to agent " << this->chr << "\n";
			plan = new Plan(search(startstate));
			Node::resetPool();
		}
		//We don't have a task/have completed
		else if(cPlanner.UnassignedTasks[this->color].size() == 0){
			//We should listen in, see if other agents request something
			//std::cerr << "No more plans\n";
			return &Command::EVERY[0];
		} else {
			//Task was completed, there's more tasks for us.
			std::cerr << "Doing replanning\n";
			//Do replanning
			delete plan;
			cPlanner.AssignTask(this);
			MoveBoxTask* tmp = reinterpret_cast<MoveBoxTask*>(this->task);
			std::cerr << "Assigned task " << tmp->box->chr << " to agent " << this->chr << "\n";
			plan = new Plan(search(startstate));
		}
	}
	//Find next step
	Command * c = plan->getStep();
	//Find number
	int number = (int)(chr - '0');



	if (!startstate->checkState(number, c)){
		std::cerr << "Conflict1!\n";
		//Do replanning next time
		plan->drain();
		return &Command::EVERY[0];
	}
	if (!tempstate->checkAndChangeState(number, c)){
		std::cerr << "Conflict2!\n";
		//Do replanning next time
		plan->drain();
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
