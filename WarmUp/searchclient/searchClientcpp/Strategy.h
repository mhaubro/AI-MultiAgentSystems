#ifndef STRATEGY_H
#define STRATEGY_H
#include <string>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <functional>
#include "memory.h"
#include "Node.h"

class Hash
{
public:
  size_t operator() (const Node * key) const
  {
    return key->hashCode();
  }
};
class Equals
{
public:
  bool operator() (Node const * t1, Node const * t2) const
  {
    return t1->hashCode() == t2->hashCode();
  }
};


class Strategy {
private:
  std::unordered_map<Node *, int, Hash, Equals> explored;
	//double startTime;

public:

	Strategy();
	void addToExplored(Node * n);
	bool isExplored(Node * n);
	int countExplored();
	std::string searchStatus();
	//double timeSpent();
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
		std::unordered_map<Node *, int> frontierSet;

		StrategyBFS();

		Node * getAndRemoveLeaf() override;
		void addToFrontier(Node * n) override;
    bool inFrontier(Node * n) override;
		int countFrontier() override;
		bool frontierIsEmpty() override;
    std::string toString() override;

	};

#endif
