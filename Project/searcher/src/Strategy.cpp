#include "Strategy.h"
#include "Memory.h"
#include <string>
#include <queue>
#include <chrono>
#include <utility>
#include <iostream>
#include <functional>
#include <unordered_set>
#include <list>
#include <cmath>

#include "Node.h"
#include "Agent.h"
#include "Task.h"
#include "Goal.h"
#include "HandleGoalTask.h"

using std::pair;
using std::vector;
using std::list;

class Frontier {
private:
	struct valued_node {
		Node* node;
		float value;
		bool operator()(const valued_node& left, const valued_node& right){
			return left.value > right.value;
		};
	};

	//auto cmp = [](valued_node left,valued_node right){return left.value < right.value;};
	std::unordered_set<Node *, NodeHash, NodeEqual > explored = std::unordered_set<Node *, NodeHash, NodeEqual >();
	std::priority_queue<valued_node, vector<valued_node>, valued_node> queue = std::priority_queue<valued_node, vector<valued_node>, valued_node>();

public:
	Frontier(){}
	~Frontier(){}

	void push(Node* n, double score){
		////std::cerr << "pushing node: value = " << score << " node = " << n << "\n" << std::flush;
		valued_node vn = {};
		vn.node = n;
		vn.value = score;
		queue.push(vn);
		explored.insert(n);
	}

	Node* pull(){
		valued_node vn = queue.top();
		queue.pop();
		////std::cerr << "Pulling best node: value = " << vn.value << " node: " << vn.node << "\n" << std::flush;
		return vn.node;
	}

	int size(){
		return queue.size();
	}

	bool empty(){
		return queue.empty();
	}

	bool is_explored(Node* n){
		bool result =  this->explored.count(n) == 1;
		return result;
	}
};

//	} else if (HandleGoalTask * t = dynamic_cast<HandleGoalTask *>(task)){
//		//std::cerr << "Not a movebox task, not supported yet\n";

list<Node*> a_star_search(Node* start_state, Agent* agent, Task* task){
	if (task == NULL)
		return std::list<Node*>();

  int MAXITE = 20000;
  if(start_state->agents.size() == 1)
      MAXITE = 40000;

	if (RequestFreeSpaceTask* tmp = dynamic_cast<RequestFreeSpaceTask*>(task))
		MAXITE = 3000;

	int iteration = 0;
	// vector holding and assuming ownership of all nodes
	std::vector<Node> explored_nodes = std::vector<Node>();
	// frontier used to select which nodes to process next.
	Frontier frontier = Frontier();

	// add start state to frontier
	frontier.push(start_state, start_state->g());

	// search loop
	while(true){
		//We shouldn't search for too long, rather we should turn around and pick other solution
		if (iteration > MAXITE){
			return std::list<Node *>();
		}
		// if frontier is empty and no solution is found, return an empty list.
		if (frontier.empty()){
			return list<Node*>();
		}
		Node* leaf = frontier.pull();

		if (iteration % 2500 == 0){
			std::cerr << "Iteration = " << iteration << "\ng = " << leaf->g() << "\nh = " << task->h(agent,leaf) << std::endl;
			std::cerr << "Searching is done in agent "<< agent->getChar() << "\n";
			//std::cerr << leaf->toString();
		}
		//std::cerr << leaf->toString() << "\n";
		if (task->seemsCompleted(agent, leaf)){
			return leaf->extractPlan();
		}
		vector<Node> new_nodes = leaf->getExpandedNodes(agent->getChar());
		for (auto& n : new_nodes){
			if (!frontier.is_explored(&n)){
				////std::cerr << "Pushing object\n" << n.toString()<< "\n";
				frontier.push(Node::getopCopy(&n), task->h(agent, &n));
			}
		}
		iteration++;
	}
}

list<Node*> a_star_search(Node* start_state, Agent* agent, Task * task, Goal * g1, Goal * g2)
{
  int MAXITE = 40000;

  HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(task);
  start_state->clearGoals(agent->getChar(), g1, g2);

  int iteration = 0;
	// vector holding and assuming ownership of all nodes
	std::vector<Node> explored_nodes = std::vector<Node>();
	// frontier used to select which nodes to process next.
	Frontier frontier = Frontier();

	// add start state to frontier
	frontier.push(start_state, start_state->g());

	// search loop
	while(true)
  {
		//We shouldn't search for too long, rather we should turn around and pick other solution
		if (iteration > MAXITE)
      return std::list<Node *>();

		// if frontier is empty and no solution is found, return an empty list.
		if (frontier.empty())
      return std::list<Node *>();

		Node* leaf = frontier.pull();

		if (leaf->isGoalState(g1))
      return leaf->extractPlan();

		vector<Node> new_nodes = leaf->getExpandedNodes(agent->getChar(), g2);
		for (auto& n : new_nodes)
    {
			if (!frontier.is_explored(&n))
				frontier.push(Node::getopCopy(&n), task->h(agent, &n));
		}
		iteration++;
	}
}
