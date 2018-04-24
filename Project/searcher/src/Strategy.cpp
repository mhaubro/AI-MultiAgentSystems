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

int Strategy::countExplored() {
	return this->explored.size();
}

void Strategy::searchStatus(int iterations) {
	//This prints the status of the search
	//Explored and frontier might be superflous later, other fields might get added
	
	std::cerr << "#Explored: "<< this->countExplored() <<" #Frontier: "<<
	this->countFrontier() << ", Time: "<< this->timeSpent() << ", " << Memory::stringRep() << "\n";
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


// ################################
// Strategy
// ################################

template<class T>
inline queue<Command> template_search(Node& StartNode, Agent& agent, Task& task){

	// container for all nodes in the search
	std::vector<Node> all_nodes = std::vector<Node>();

	T frontier = T();
	frontier.push(StartNode);

	bool solution_found = false;
	while (!frontier.empty()){
		Node* current = pop(frontier);

		if (task.achived(node)){
			solution_found = true;
			break;
		}

		// push neighbour nodes to frontier and all nodes vector
		for (Command c :Command::EVERY){
			all_nodes.push(Node(parent))
			frontier.push
		}



	}

	//

}

// ################################
// BFS
// ################################

class BFS_Frontier {
public:
	BFS_Frontier();
	~BFS_Frontier();

	void push(Node* node);
	Node* pop();

private:
	std::queue<Node*> queue;
	std::unordered_set<Node*> node_set;
}

BFS_Frontier::BFS_Frontier(){
	queue = std::queue<Node*>();
	node_set = std::unordered_set<Node*>();
}

BFS_Frontier::push(Node* node){
	queue.push(node);
	node_set.insert(node);
}

BFS_Frontier::pop(){
	Node * n = frontier.front();
	frontier.pop();
	frontierSet.erase(n);
	return n;
}

queue<Command> BFS::search(Node& StartNode, Agent& agent, Task& task){
	return template_search<BFS_Frontier>(StartNode, agent, task);
}


// ################################
// AStar
// ################################