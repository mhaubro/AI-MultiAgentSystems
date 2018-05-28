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
			if (a->getChar() != chr){
				//std::cerr << "Putting out agent temporarily\n";
				Node stateWithoutAgent = *state;
				stateWithoutAgent.removeAgent(tmp->destination);

				//This should be a function
				std::list<Location> loc = std::list<Location>();
				loc.push_back(tmp->destination);
				myPlanner->removeRequestTask(freeSpaceTask);
				RequestFreeSpaceTask * req = new RequestFreeSpaceTask(loc, 0, this, tmp->box);
				if (freeSpaceTask && (req->locations == freeSpaceTask->locations)){
					delete req;
				} else {
					delete freeSpaceTask;
					myPlanner->removeRequestTask(freeSpaceTask);
					freeSpaceTask = req;
					myPlanner->addRequestFreeSpaceTask(freeSpaceTask);
				}
				return a_star_search(&stateWithoutAgent, this, this->task, 20000);
			}
		}
		if (Box * b = state->getBox(tmp->destination)){
			if (b->getColor() != color){
				//std::cerr << "Putting out box temporarily\n";
				Node stateWithoutBox = *state;
				stateWithoutBox.removeBox(tmp->destination);

				//This should be a function
				std::list<Location> loc = std::list<Location>();
				loc.push_back(tmp->destination);
				myPlanner->removeRequestTask(freeSpaceTask);
				RequestFreeSpaceTask * req = new RequestFreeSpaceTask(loc, 0, this, tmp->box);
				if (freeSpaceTask && (req->locations == freeSpaceTask->locations)){
					delete req;
				} else {
					delete freeSpaceTask;
					myPlanner->removeRequestTask(freeSpaceTask);
					freeSpaceTask = req;
					myPlanner->addRequestFreeSpaceTask(freeSpaceTask);
				}


				return a_star_search(&stateWithoutBox, this, this->task, 20000);
				//There's a box we can't move at destination
			}
		}
	}
	return a_star_search(state, this, this->task, 20000);
}

//Commits a search where all agents are gone, and asks for the locations.
std::list<Node *> Agent::Nakedsearch(Node * state){
	Node nakedstate = *state;
	nakedstate.clearOtherAgents(chr);

	return search(&nakedstate);
}

//Commits a search where all things are gone, and asks for the locations.
std::list<Node *> Agent::noBoxesOrAgents(Node * state, Box * box){
	Node nakedstate = *state;
	nakedstate.clearOtherAgentsAndBoxes(chr, box);
	return search(&nakedstate);
}

void Agent::cleanTasks(){
	if (HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(this->task)){
		myPlanner->returnGoalTask(tmp);
		tmp->box->workInProgress = false;
	} else 	if (RequestFreeSpaceTask* tmp = dynamic_cast<RequestFreeSpaceTask*>(this->task)){
		if (!myPlanner->stillActiveRequest(tmp)){
			task = NULL;
			std::cerr << "We have ourselves a bug\n";
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

	if (RequestFreeSpaceTask * tmp = dynamic_cast<RequestFreeSpaceTask*>(this->task)){
		std::cerr << "We tried to help\n";
		//We won't be bothered
		this->task = NULL;
		return;
	} else if (HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(this->task)){
		//Takes out all agents
		std::cerr << "We're trying to fix a goal\n";
		searchResult = Nakedsearch(state);

		//No boxes or agents, try path
		if (searchResult.empty()){
			searchResult = noBoxesOrAgents(state, tmp->box);
			if (searchResult.empty()){
				myPlanner->returnGoalTask((HandleGoalTask *)task);
				this->task = NULL;
				return;
			}
			std::cerr << "Found a path without boxes or agents\n";
			//We have a path without agents and boxes, we proceed
		}
		//We have a path without agents, we proceed
		plan = new Plan(searchResult, this->getLocation());

		RequestFreeSpaceTask * req = new RequestFreeSpaceTask(plan->getLocations(), rank, this, tmp->box);
		if (freeSpaceTask && (req->locations == freeSpaceTask->locations)){
			delete req;
		} else {
			delete freeSpaceTask;
			myPlanner->removeRequestTask(freeSpaceTask);
			freeSpaceTask = req;
			myPlanner->addRequestFreeSpaceTask(freeSpaceTask);
		}

	}
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

	if (task == NULL){
		std::cerr << "My task is null\n";
		task = myPlanner->getJob(this, startstate);
	}

	if (HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(this->task)){
		std::cerr << "This is a handleGoalTask\n";
	}

	//If we still don't have a task
	if (task == NULL){
		std::cerr << "My task is still null\n";
		task = new GetAwayFromGoalTask();
	}

	std::list<Node *> searchResult = search(startstate);

	if (searchResult.empty()){
		std::cerr << "A solution was not found\n";

		//We check if we can help. If not, we ask for help ourselfes.
		if (checkForHelp(startstate)){
			//We can help. Let's help.
			searchResult = search(startstate);
			plan = new Plan(searchResult, location);
		} else {
			//Let's see if we can solve our current task.
			replanTask(startstate);
		}


	} else {
		std::cerr << "A solution was found\n";
		//We found a way to handle our current task.
		delete plan;
		removeFreeSpaceTask();
		plan = new Plan(searchResult, location);
	}
}

void Agent::maybeSleep(int sleep){
		double prob = 0.3;
		if (((double)rand())/RAND_MAX + prob > 1){
			skipNextIte = sleep;
		}
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
	//Checks if we have a plan, and the task is still valid
	if (plan != NULL && !plan->isEmpty() && task != NULL){
		//Plan seems to be legal
		Command * c = plan->getStep();
		if (c == NULL){
			//This should be a sanity check
			plan->drain();
			return &Command::EVERY[0];
		}

		if (!startstate->checkState(number, c)){
			noPlan(tempstate);
			maybeSleep(2);
			return &Command::EVERY[0];
		}
		if (!tempstate->checkAndChangeState(number, c)){
			noPlan(tempstate);
			maybeSleep(2);
			return &Command::EVERY[0];
		}

		plan->popStep();
		return c;

	} else {
		//We did not have a legal plan
		noPlan(tempstate);
	}

	return &Command::EVERY[0];
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
