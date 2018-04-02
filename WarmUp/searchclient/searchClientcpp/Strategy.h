#ifndef STRATEGY_H
#define STRATEGY_H
#include <string>
#include <unordered_map>
#include <queue>
#include <chrono>
#include "memory.h"
#include "Node.h"

namespace std {

  template <>
  struct hash<Node *>
  {
    std::size_t operator()(const Node& k) const
    {
      	return (k.hashCode());
		}
  };

}

class Strategy {
private:
	std::unordered_map<Node *, int> explored;
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

		Node * getAndRemoveLeaf();
		void addToFrontier(Node * n);
    bool inFrontier(Node * n);
		int countFrontier();
		bool frontierIsEmpty();
    std::string toString();

	};

#endif
