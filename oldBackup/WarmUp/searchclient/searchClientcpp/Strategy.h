#ifndef STRATEGY_H
#define STRATEGY_H
#include <string>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <functional>
#include <unordered_set>
#include "memory.h"
#include "Node.h"

class Strategy {
private:
  std::unordered_set<Node *, NodeHash, NodeEqual> explored;
	std::chrono::high_resolution_clock::time_point startTime;

public:

  double timeSpent();
	Strategy();
	void addToExplored(Node * n);
	bool isExplored(Node * n);
	int countExplored();
  void searchStatus(int iterations);	//double timeSpent();
	virtual Node * getAndRemoveLeaf();
	virtual void addToFrontier(Node * n);
	virtual bool inFrontier(Node * n);
	virtual int countFrontier();
	virtual bool frontierIsEmpty();
	virtual std::string toString();

};

	class StrategyBFS : public Strategy {
  public:
		std::queue<Node *> frontier;
		std::unordered_set<Node *, NodeHash, NodeEqual> frontierSet;

		StrategyBFS();

		Node * getAndRemoveLeaf() override;
		void addToFrontier(Node * n) override;
    bool inFrontier(Node * n) override;
		int countFrontier() override;
		bool frontierIsEmpty() override;
    std::string toString() override;

	};

#endif
