#ifndef STRATEGY_H
#define STRATEGY_H
#include "centralHeader.h"
#include <string>
#include <unordered_set>
#include "memory.h"
#include "Node.h"

class Strategy {
private:
	std::unordered_set<Node> explored;
	long startTime;

public:

	Strategy();
	void addToExplored(Node n);
	bool isExplored(Node n);
	int countExplored();
	std::string searchStatus();
	float timeSpent();
	virtual Node getAndRemoveLeaf();
	virtual void addToFrontier(Node n);
	virtual bool inFrontier(Node n);
	virtual int countFrontier();
	virtual bool frontierIsEmpty();
	virtual std::string toString();

};
	class StrategyBFS : public Strategy {
		std::queue<Node> frontier;
		std::unordered_set<Node> frontierSet;

		StrategyBFS();

		Node getAndRemoveLeaf();
		void addToFrontier(Node n);
		int countFrontier();
		bool frontierIsEmpty();
		bool inFrontier(Node n);
    std::string toString();
	};

#endif
