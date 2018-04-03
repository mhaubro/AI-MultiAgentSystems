#include "Strategy.h"
#include "Memory.h"
#include <string>
#include <queue>
#include <chrono>
#include <utility>
#include <iostream>



	Strategy::Strategy() {
		this->explored = std::unordered_map<Node *, int, Hash, Equals >();
		//this->startTime = std::chrono::high_resolution_clock::now();
	}

	void Strategy::addToExplored(Node * n) {
		this->explored.insert(std::make_pair(n, n->hashCode()));
	}

	bool Strategy::isExplored(Node * n) {
		std::cerr << "H12!";
		bool result =  this->explored.count(n) == 1;
		return result;
	}

	int Strategy::countExplored() {
		return this->explored.size();
	}

	std::string Strategy::searchStatus() {
		char stemp[100];
		sprintf(stemp, "#Explored: %,6d, #Frontier: %,6d, #Generated: %,6d, Time: %3.2f s \t%s", this->countExplored(), this->countFrontier(), this->countExplored()+this->countFrontier(), 0.0, Memory::stringRep());
		std::string s;
		s.resize(sizeof(stemp)-1);
		memcpy(&s[0], stemp, strlen(stemp)-1);
		return s;
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
	/*double Strategy::timeSpent() {
		return (duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - this->startTime)).count();
	}*/


	StrategyBFS::StrategyBFS() {
		std::queue<Node *> frontier = std::queue<Node *>();
		std::unordered_map<Node *, int> frontierSet = std::unordered_map<Node *, int>();
	}

		Node * StrategyBFS::getAndRemoveLeaf() {
			Node * n = frontier.front();
			frontierSet.erase(n);
			return n;
		}

		void StrategyBFS::addToFrontier(Node * n) {
			frontier.push(n);
			frontierSet.insert(std::make_pair(n, n->hashCode()));
		}

		int StrategyBFS::countFrontier() {
			return frontier.size();
		}

		bool StrategyBFS::frontierIsEmpty() {
			return frontier.empty();
		}

		bool StrategyBFS::inFrontier(Node * n) {
			std::cerr << "H6!";
			return frontierSet.count(n);
		}

    std::string StrategyBFS::toString() {
			return std::string("Breadth-first Search");
		}
