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
	if (HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(this->task)){
		//There's an agent on our destination
		if (state->getAgent(tmp->destination)){
			Node stateWithoutAgent = *state;
			stateWithoutAgent.removeAgent(tmp->destination);
			return a_star_search(&stateWithoutAgent, this, this->task);
		}
		if (Box * b = state->getBox(tmp->destination)){
			if (b->getColor() != color){
				Node stateWithoutBox = *state;
				stateWithoutBox.removeBox(tmp->destination);
				return a_star_search(&stateWithoutBox, this, this->task);
				//There's a box we can't move at destination
			}
		}
	}
	return a_star_search(state, this, this->task);
}

//Commits a search where all things are gone, and asks for the locations.
std::list<Node *> Agent::Nakedsearch(Node * state){
	Node nakedstate = *state;
	nakedstate.clearOtherAgents(chr);
	return a_star_search(state, this, this->task);
}

//Commits a search where all things are gone, and asks for the locations.
std::list<Node *> Agent::noBoxesOrAgents(Node * state, Box * box){
	Node nakedstate = *state;
	nakedstate.clearOtherAgentsAndBoxes(chr, box);
	return a_star_search(state, this, this->task);
}

Command * Agent::noPlan(Node * startstate){
	std::cerr<< "Doing NoPlan from agent" <<  chr << "\n";

	if (this->task != NULL && this->task->seemsCompleted(this, startstate)){
		//Task was completed, there's more tasks for us.
		std::cerr <<"Task was: " << task << " Doing replanning\n" ;
		std::cerr << "Being returned from agent "<< chr<< "\n";

		if (HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(this->task)){
			myPlanner->returnGoalTask(tmp);
			myPlanner->removeRequestTask(t);
		}
    else if (RequestFreeSpaceTask* tmp = dynamic_cast<RequestFreeSpaceTask*>(this->task))
    {
			if (!myPlanner->stillActiveRequest(tmp))
				task = NULL;
		}
	}

	//Short-cirrcuit. We have a task, which is not completed
	if (this->task != NULL && !this->task->seemsCompleted(this, startstate)){
		//Task wasn't completed, let's replan
		std::cerr << "Doing replanning with original task\n";
		//Do replanning
		//delete plan;
		std::list<Node *> searchResult = search(startstate);

		if (searchResult.empty()){
			std::cerr << "Search Result was empty\n";
			//No agents, try path
			if (RequestFreeSpaceTask * tmp = dynamic_cast<RequestFreeSpaceTask*>(this->task)){
				return &Command::EVERY[0];
			}
			if (HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(this->task)){
				//				//std::cerr << "Assigned task " << tmp->box->chr << " to agent " << this->chr << "\n";
				searchResult = Nakedsearch(startstate);

				//No boxes or agents, try path
				if (searchResult.empty()){
					searchResult = noBoxesOrAgents(startstate, tmp->box);
					if (searchResult.empty()){
						//We return the task to the centralplanner, and see if it has a helpjob
						myPlanner->removeRequestTask(t);
						//delete t;
						t = NULL;
						myPlanner->returnGoalTask((HandleGoalTask *)task);
						this->task = NULL;
						return &Command::EVERY[0];
					}
					//We have a path without agents and boxes, we proceed
				}
				//We have a path without agents, we proceed
			}

			//std::cerr << "Wanting to add a request\n";
			plan = new Plan(searchResult, this->getLocation());
			t = new RequestFreeSpaceTask(plan->getLocations(), rank);
			//Do something
			myPlanner->addRequestFreeSpaceTask(t);
			//std::cerr << "NoCrash?\n";
			Node::resetPool();
			return NULL;
		}

		plan = new Plan(searchResult, this->getLocation());
		std::cerr << "Following Plan found\n";
		std::cerr << plan->toString() << "\n";
		Node::resetPool();
		return NULL;
	}
	//We don't have a task/have completed
	else if (!myPlanner->hasJob(this, startstate)){
		//Noone has requested anything.
		//Maybe we should improve our positioning, by moving away from other agents??
		return &Command::EVERY[0];
	} else {
		//Remove any requests for more space. If it's null, doesn't matter.
		myPlanner->removeRequestTask(t);
		//delete t;
		t = NULL;
		task = NULL;

		//Do replanning
		//delete plan;

		if (myPlanner->hasJob(this, startstate)){
			task = myPlanner->getJob(this, startstate);
			if (HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(this->task)){
				//std::cerr << "Goal: "<< tmp->chr << " destination: " << tmp->destination.first <<"," << tmp->destination.second << " box: " << tmp->box->chr << "\n";
			}
			std::list<Node *> searchResult = search(startstate);
			//std::cerr << "Received a task\n";
			if (searchResult.empty()){
				//std::cerr << "Empty plan?\n";
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
		//myPlanner->removeTask(t);
		skipNextIte = 2;
	} else {
		if (plan != NULL && !(plan->locations.empty())){
			myPlanner->removeRequestTask(t);
			//delete t;
			t = new RequestFreeSpaceTask(plan->locations, rank);
		}
	}
	double prob = 0.3;
	if (((double)rand())/RAND_MAX + prob > 1)
		skipNextIte = 2;
	////std::cerr << "Conflict1!\n";
	//Do replanning next time
	plan->drain();
	return &Command::EVERY[0];
}

Command * Agent::getAction(Node * startstate, Node * tempstate){
	if (skipNextIte){
		skipNextIte--;
		return &Command::EVERY[0];
	}
	if (plan == NULL || plan->isEmpty()){
		if (Command * c = noPlan(startstate)){
			std::cerr << "C is being returned from agent "<< chr<< "\n";
			return c;
		}
	}
	//Find next step
	//std::cerr << "Getting a step\n";
	Command * c = plan->getStep();
	std::cerr << "Getting a step from agent " << chr << " cmd: " << c->toString() << "\n";
	if (c == NULL){
		plan->drain();
		return &Command::EVERY[0];
	}
	//Find number
	//int number = (int)(chr - '0');

	if (!startstate->checkState(number, c)){
		std::cerr << "CheckState failed for " << c->toString() << " is being returned from agent "<< chr<< "\n";
		return handleConflict();
	}
	if (!tempstate->checkAndChangeState(number, c)){
		std::cerr << "CheckandchangeState failed for " << c->toString() << " is being returned from agent "<< chr<< "\n";
		return handleConflict();
	}
	plan->popStep();
	std::cerr << c->toString() << " is being returned from agent "<< chr<< "\n";
	return c;
}

Agent::Agent(char chr, int rank, Location location, COLOR color, int region):
Entity(chr, location, color, region)
{
	this->task = nullptr;
	this->rank = rank;
	this->number = (int)(chr - '0');
	this->myPlanner = nullptr;
	plan = NULL;
	t = NULL;

}

Agent::Agent(char chr, Location location, COLOR color, int region):
Entity(chr, location, color, region)
{
	this->task = nullptr;
	this->rank = 0;
	this->number = chr - '0';
	this->myPlanner = nullptr;
	plan = NULL;
	t = NULL;
}

//No color, for single agent levels
Agent::Agent(char chr, Location location, int region):
Entity(chr, location, Entity::BLUE, region)
{
	this->task = nullptr;
	this->rank = 0;
	this->number = chr - '0';
	this->myPlanner = nullptr;
	plan = NULL;
	t = NULL;
}

Agent::Agent(const Agent * agt):
Entity(agt->getChar(), agt->getLocation(), agt->getColor(), agt->getRegion())
{
	this->task = agt->task;
	this->rank = agt->rank;
	this->plan = agt->plan;
	this->number = chr - '0';
	this->myPlanner = agt->getPlanner();
	std::cerr << "centralplanner from " << myPlanner->region << "\n";
	t = NULL;
}

int Agent::hashCode()
{
	int result = 17;
	result = 31 * result + (int) chr;
	result = 31 * result + rank;
	result = 31 * result + location.getX();
	result = 31 * result + location.getY();
	result = 31 * result + color;
	return result;
}

void Agent::setMyPlanner(CentralPlanner * planner){
	myPlanner = planner;
}

CentralPlanner * Agent::getPlanner() const{
	return myPlanner;
}

bool Agent::equals(const Agent * agent) const
{
	if (agent == this) return true;
	return agent->getChar() == chr && agent->rank == rank && agent->getColor() == color && agent->getLocation() == location;
}
