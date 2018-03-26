#include "Strategy.h"
#include "centralHeader.h"
#include <string>
#include <queue>
#include <unordered_set>

namespace std
{
  template<>
    struct hash<Node>
    {
      size_t
      operator()(const Student & obj) const
      {
        return obj.hashCode();
      }
    };
}


	Strategy::Strategy() {
		this->explored = new std::unordered_set<Node>();
		this->startTime = System.currentTimeMillis();
	}

	void Strategy::addToExplored(Node n) {
		this->explored.add(n);
	}

	bool Strategy::isExplored(Node n) {
		return this->explored.contains(n);
	}

	int Strategy::countExplored() {
		return this->explored.size();
	}

	std::string Strategy::searchStatus() {
		char stemp[100];
		sprintf(stemp, "#Explored: %,6d, #Frontier: %,6d, #Generated: %,6d, Time: %3.2f s \t%s", this->countExplored(), this->countFrontier(), this->countExplored()+this->countFrontier(), this->timeSpent(), Memory->stringRep());
		std::string s;
		s.resize(sizeof(stemp)-1);
		memcpy(&s[0], stemp, strlen(stemp)-1);
		return s;
	}

	float Strategy::timeSpent() {
		return (System.currentTimeMillis() - this->startTime) / 1000f;
	}

	std::queue<Node> StrategyBFS::frontier = ;
	std::unordered_set<Node, NodeHashser, NodeComparator> StrategyBFS::frontierSet = ;

	StrategyBFS::StrategyBFS() {
		frontier = new std::queue<Node>();
		frontierSet = new std::unordered_set<Node, NodeHashser, NodeComparator>();
	}

		Node StrategyBFS::getAndRemoveLeaf() {
			Node n = frontier.pollFirst();
			frontierSet.remove(n);
			return n;
		}

		void StrategyBFS::addToFrontier(Node n) {
			frontier.addLast(n);
			frontierSet.add(n);
		}

		int StrategyBFS::countFrontier() {
			return frontier.size();
		}

		bool StrategyBFS::frontierIsEmpty() {
			return frontier.isEmpty();
		}

		bool StrategyBFS::inFrontier(Node n) {
			return frontierSet.contains(n);
		}

    std::string StrategyBFS::toString() {
			return std::string("Breadth-first Search");
		}
