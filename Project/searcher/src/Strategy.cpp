#include "Strategy.h"
#include "Memory.h"
#include <string>
#include <queue>
#include <chrono>
#include <utility>
#include <iostream>

#include <ratio>

	Strategy::Strategy() {
		this->explored = std::unordered_set<Node *, NodeHash, NodeEqual >();
		this->startTime = std::chrono::high_resolution_clock::now();
	}

	void Strategy::addToExplored(Node * n) {
		this->explored.insert(n);
	}

	bool Strategy::isExplored(Node * n) {
		bool result =  this->explored.count(n) == 1;
		return result;
	}

double getDistance(Box b, std::pair<int, int> location){
	return std::sqrt(std::pow(b.getX()-location.first,2) + std::pow(b.getY()-location.second,2));
}

double getHValue(Node * n, Agent * agent, Task * task){
	double hval = 1000.0;
	//Find box.
	//Yes, the if below is legal
	if (HandleGoalTask * t = dynamic_cast<HandleGoalTask *>(task)){
		//std::cerr << "Doing a heuristic thing\n";
		Box * box;
		//IT's a moveboxtask
		//MoveBoxTask * t = dynamic_cast<MoveBoxTask *>( task );
		for (Box b : n->boxes){
			if (b.color != agent->color)//We don't care
				continue;
			//For all boxes with matching color, deduce like 3 if they're on the right place
			if (Goal * g = n->getGoal(b.getX(), b.getY())){
				if (g->chr == tolower(b.chr)){
					if (g->getLocation() == b.getLocation()){
						//std::cerr << "Goal Match" << b.chr << "\n";
						hval -= 5.0;
						//std::cerr << n->toString() << "\n";
					} else {

					}
				}
			}
			if (b.chr == t->box->chr){
				hval += getDistance(b, t->destination);

				if (getDistance(b, n->agents[agent->chr - '0'].getLocation()) < 1.3){//Ensures they're next to
					hval -= 5.0;
					//std::cerr << "Next to box" << b.chr << " Position: " << b.getX() <<"," <<b.getY() << "\n";
					//std::cerr << "Destination" << " Position: " << t->destination.first <<"," <<t->destination.second << "\n";
				} else {
					hval += getDistance(b, n->agents[agent->chr - '0'].getLocation());
				}

				//The right box
			}
		}
	} else if (HandleGoalTask * t = dynamic_cast<HandleGoalTask *>(task)){
		std::cerr << "Not a movebox task, not supported yet\n";
	}


	return hval+n->g() ;
}

list<Node*> a_star_search(Node* start_state, Agent* agent, Task* task){

	int iteration = 0;
	// vector holding and assuming ownership of all nodes
	std::vector<Node> explored_nodes = std::vector<Node>();
	// frontier used to select which nodes to process next.
	Frontier frontier = Frontier();

	// add start state to frontier
	frontier.push(start_state, start_state->g());

	// search loop
	while(true){

		// if frontier is empty and no solution is found, return an empty list.
		if (frontier.empty()){
			return list<Node*>();
		}
		Node* leaf = frontier.pull();
		//std::cerr << leaf->toString() << "\n";
		if (task->seemsCompleted(agent, leaf)){
			return leaf->extractPlan();
		}
		vector<Node> new_nodes = leaf->getExpandedNodes(agent->chr);
		for (auto& n : new_nodes){
			if (!frontier.is_explored(&n)){
				//std::cerr << "Pushing object\n" << n.toString()<< "\n";
				frontier.push(Node::getopCopy(&n), getHValue(&n, agent, task));
			}
		}
		iteration++;
	}

	Node * Strategy::getAndRemoveLeaf(){
		return (Node *) NULL;
	}
	void Strategy::addToFrontier(Node * n){	}

	bool Strategy::inFrontier(Node * n){
		return false;
	}

	int Strategy::countFrontier(){
		return 1;
	}
	bool Strategy::frontierIsEmpty(){
		return false;
	}
	std::string Strategy::toString(){
		return std::string("");
	}
	double Strategy::timeSpent() {
		std::chrono::duration<double> time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - startTime);
		return time.count();
	}


	StrategyBFS::StrategyBFS() {
		this->frontier = std::queue<Node *>();
		this->frontierSet = std::unordered_set<Node *, NodeHash, NodeEqual>();
	}

		Node * StrategyBFS::getAndRemoveLeaf() {
			Node * n = frontier.front();
			frontier.pop();
			frontierSet.erase(n);
			return n;
		}

		void StrategyBFS::addToFrontier(Node * n) {
			frontier.push(n);
			frontierSet.insert(n);
		}

		int StrategyBFS::countFrontier() {
			return frontier.size();
		}

		bool StrategyBFS::frontierIsEmpty() {
			return frontier.empty();
		}

		bool StrategyBFS::inFrontier(Node * n) {
			return frontierSet.count(n) == 1;
		}

    std::string StrategyBFS::toString() {
			return std::string("Breadth-first Search");
		}
