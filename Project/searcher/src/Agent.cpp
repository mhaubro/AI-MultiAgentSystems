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
#include "GetAwayFromGoalTask.h"
#include <iostream>

std::list<Node *> Agent::search(Node * state){
	if (HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(this->task)){
		//There's an agent on our destination
		if (Agent * a = state->getAgent(tmp->destination)){
			//std::cerr << "Putting out agent temporarily\n";
			Node stateWithoutAgent = *state;
			stateWithoutAgent.removeAgent(tmp->destination);

			//This should be a function
			myPlanner->removeRequestTask(freeSpaceTask);
			delete freeSpaceTask;
			std::list<Location> loc = std::list<Location>();
			loc.push_back(tmp->destination);
			freeSpaceTask = new RequestFreeSpaceTask(loc, 0, tmp->box);
			myPlanner->addRequestFreeSpaceTask(freeSpaceTask);

			return a_star_search(&stateWithoutAgent, this, this->task, 20000);
		}
		if (Box * b = state->getBox(tmp->destination)){
			if (b->getColor() != color){
				//std::cerr << "Putting out box temporarily\n";
				Node stateWithoutBox = *state;
				stateWithoutBox.removeBox(tmp->destination);

				//This should be a function
				myPlanner->removeRequestTask(freeSpaceTask);
				delete freeSpaceTask;
				std::list<Location> loc = std::list<Location>();
				loc.push_back(tmp->destination);
				freeSpaceTask = new RequestFreeSpaceTask(loc, 0, tmp->box);
				myPlanner->addRequestFreeSpaceTask(freeSpaceTask);

				return a_star_search(&stateWithoutBox, this, this->task, 20000);
				//There's a box we can't move at destination
			}
		}
	}
	return a_star_search(state, this, this->task, 20000);
}

//Commits a search where all things are gone, and asks for the locations.
std::list<Node *> Agent::Nakedsearch(Node * state){
	Node nakedstate = *state;
	nakedstate.clearOtherAgents(chr);
	return a_star_search(state, this, this->task, 20000);
}

//Commits a search where all things are gone, and asks for the locations.
std::list<Node *> Agent::noBoxesOrAgents(Node * state, Box * box){
	Node nakedstate = *state;
	nakedstate.clearOtherAgentsAndBoxes(chr, box);
	return a_star_search(state, this, this->task, 20000);
}

void Agent::cleanTasks(){
	//Task was completed, there's more tasks for us. We do some cleaning.
	//std::cerr <<"Task was: " << task << " Doing replanning\n" ;
	//std::cerr << "Being returned from agent "<< chr<< "\n";

	if (HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(this->task)){
		myPlanner->returnGoalTask(tmp);
	} else 	if (RequestFreeSpaceTask* tmp = dynamic_cast<RequestFreeSpaceTask*>(this->task)){
		if (!myPlanner->stillActiveRequest(tmp)){
			task = NULL;
			std::cerr << "We have ourselefs a bug\n";
		}
	} else 	if (GetAwayFromGoalTask* tmp = dynamic_cast<GetAwayFromGoalTask*>(this->task)){
		delete task;
		task = NULL;
	}
	removeFreeSpaceTask();
	//t should be deleted, but that would break everything right now
	//TODO

}

void Agent::removeFreeSpaceTaskMessage(RequestFreeSpaceTask * t){
	if (task == t){
		task = NULL;
		plan->drain();
	}
}

void Agent::removeFreeSpaceTask(){
	myPlanner->removeRequestTask(freeSpaceTask);
	delete freeSpaceTask;
	freeSpaceTask = NULL;
}

void Agent::replanTask(Node * state){
	std::cerr << "Replanning from agent " << chr << "\n";
	std::list<Node *> searchResult = search(state);

	if (searchResult.empty()){
		std::cerr << "Search Result was empty from agent " << chr << "\n";
		//No agents, try path
		if (RequestFreeSpaceTask * tmp = dynamic_cast<RequestFreeSpaceTask*>(this->task)){
			this->task = NULL;
			return;
		} else if (HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(this->task)){
			//Takes out all agents
			searchResult = Nakedsearch(state);

			//No boxes or agents, try path
			if (searchResult.empty()){
				searchResult = noBoxesOrAgents(state, tmp->box);
				if (searchResult.empty()){
					myPlanner->returnGoalTask((HandleGoalTask *)task);
					this->task = NULL;
					return;
				}
				//We have a path without agents and boxes, we proceed
			}
			//We have a path without agents, we proceed
			plan = new Plan(searchResult, this->getLocation());
			freeSpaceTask = new RequestFreeSpaceTask(plan->getLocations(), rank, tmp->box);
			myPlanner->addRequestFreeSpaceTask(freeSpaceTask);
			Node::resetPool();
			return;
		}
	}

	plan = new Plan(searchResult, this->getLocation());
	std::cerr << "Following Plan found\n";
	std::cerr << plan->toString() << "\n";
	Node::resetPool();

}

void Agent::noTask(Node * startstate){
	task = new GetAwayFromGoalTask();
	plan = new Plan(search(startstate), this->getLocation());
}

void Agent::gettingJob(Node * startstate){

	if (myPlanner->hasJob(this, startstate)){
		task = myPlanner->getJob(this, startstate);
		if (HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(this->task)){
			//std::cerr << "Goal: "<< tmp->chr << " destination: " << tmp->destination.first <<"," << tmp->destination.second << " box: " << tmp->box->chr << "\n";
		}
		std::list<Node *> searchResult = search(startstate);
		//std::cerr << "Received a task\n";
		if (searchResult.empty()){
			//std::cerr << "Empty plan?\n";
			return;
			//Do something
		}
		plan = new Plan(searchResult, this->getLocation());
	}
}

void Agent::noPlan(Node * startstate){
	std::cerr << "Doing noPlan from agent" << chr << "\n";

	if (this->task != NULL && this->task->seemsCompleted(this, startstate)){
		//We are done with the current task
		std::cerr << "Task seems completed from agent" << chr << "\n";
		cleanTasks();
	}

	//Short-circuit. We have a task, which is not completed. We replan.
	if (this->task != NULL && !this->task->seemsCompleted(this, startstate)){
		std::cerr << "Task will be replanned from agent" << chr << "\n";
		return replanTask(startstate);
	}

	//We don't have a task/have completed
	else if(!myPlanner->hasJob(this, startstate)){
		return noTask(startstate);
	} else {
		return gettingJob(startstate);
	}
}

Command * Agent::handleConflict(Node * state){
	if (HandleGoalTask * tmp = dynamic_cast<HandleGoalTask *>(task)){
		if (plan != NULL && !(plan->locations.empty())){
			myPlanner->removeRequestTask(freeSpaceTask);
			freeSpaceTask = new RequestFreeSpaceTask(plan->locations, rank);
			myPlanner->addRequestFreeSpaceTask(freeSpaceTask);
		}
	} else {

	}
	double prob = 0.4;
	if (((double)rand())/RAND_MAX + prob > 1){
		skipNextIte = 2;
		plan->drain();
	}

	//std::cerr << "Conflict1!\n";
	//Do replanning next time
	return &Command::EVERY[0];
}

bool Agent::checkForHelp(Node * state){
	if (myPlanner->hasHelpJob(this, state) && !myPlanner->stillActiveRequest((RequestFreeSpaceTask *) task)){
		std::cerr << "My helpjob exists\n";
		cleanTasks();
		task = myPlanner->getHelpJob(this, state);
		plan->drain();
		return true;
	}
	return false;
}

Command * Agent::getAction(Node * startstate, Node * tempstate){
	std::cerr << "Gets action for agent " << chr << "\n";
	if (skipNextIte){
		skipNextIte--;
		return &Command::EVERY[0];
	}

	checkForHelp(tempstate);

	//Checks if we have a plan, and the task is still valid
	if (plan == NULL || plan->isEmpty() || task == NULL){
		std::cerr << "Trying to plan\n";
		noPlan(startstate);
	}

	std::cerr << plan->toString();

	if (HandleGoalTask * tmp = dynamic_cast<HandleGoalTask *>(task)){
		//std::cerr << "Trying to fix goal to destination " << tmp->destination <<"\n";
	} else if (RequestFreeSpaceTask * tmp = dynamic_cast<RequestFreeSpaceTask *>(task)){
		std::cerr << "Trying to help\n";
	} else {
		//std::cerr << "I should do a NoOp\n";
	}

	if (plan == NULL || plan->isEmpty()){
		std::cerr << "Bug1\n";
		cleanTasks();
		return &Command::EVERY[0];
	}

	Command * c = plan->getStep();
	if (c == NULL){
		std::cerr << "Bug2\n";
		//Just a sanity check
		plan->drain();
		return &Command::EVERY[0];
	}

	if (!startstate->checkState(number, c)){
		return handleConflict(tempstate);
	}
	if (!tempstate->checkAndChangeState(number, c)){
		return handleConflict(tempstate);
	}

	plan->popStep();
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
	freeSpaceTask = NULL;

}

Agent::Agent(char chr, Location location, COLOR color, int region):
		Entity(chr, location, color, region)
{
	this->task = nullptr;
	this->rank = 0;
	this->number = chr - '0';
	this->myPlanner = nullptr;
	plan = NULL;
	freeSpaceTask = NULL;
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
	freeSpaceTask = NULL;
}

Agent::Agent(const Agent * agt):
		Entity(agt->getChar(), agt->getLocation(), agt->getColor(), agt->getRegion())
{
	this->task = agt->task;
	this->rank = agt->rank;
	this->plan = agt->plan;
	this->number = chr - '0';
	this->myPlanner = agt->getPlanner();
	freeSpaceTask = NULL;
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
