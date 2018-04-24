#include "Agent.h"
#include "Task.h"
#include <functional>
#include <utility>
#include <boost/pool/object_pool.hpp>
#include "Entity.h"
#include "Strategy.h"
#include <iostream>
//boost::object_pool<Agent> Agent::pool;

std::list<Node *> Agent::search(Node * state){
  int printfrequency = 10000;
  StrategyBFS * strategy = new StrategyBFS();

  char buffer[100];
  sprintf(buffer, "Search starting with strategy %s.\n", strategy->toString().c_str());
  std::string s = std::string(buffer);
  std::cerr << s;
  strategy->addToFrontier(state);
  int iterations = 0;

  while (true) {
    if (iterations % printfrequency == 0) {
      strategy->searchStatus(iterations);
    }

    if (strategy->frontierIsEmpty()) {
      //Empty list
      return std::list<Node *>();
    }

    Node * leafNode = strategy->getAndRemoveLeaf();

    if (leafNode->isGoalState(this->color)) {
      //A goal is found, final state is printed
      std::cerr << "Goal!\n";
      strategy->searchStatus(iterations);
      std::cerr << leafNode->toString();
      return leafNode->extractPlan();
    }

    strategy->addToExplored(leafNode);
    //Gets all new nodes
    std::vector<Node> nodes = leafNode->getExpandedNodes(this->chr);
    for (auto & n : nodes) {
      if (!strategy->isExplored(&n) && !strategy->inFrontier(&n)) {
        strategy->addToFrontier(Node::getopCopy(&n));
      }
    }
    iterations++;
  }
}

Agent::Agent(char chr, int rank, std::pair<int, int> location, COLOR color):
Entity(chr, location, color){
  this->rank = rank;
}

Agent::Agent(char chr, std::pair<int, int> location, COLOR color):
Entity(chr, location, color)
{
  this->rank = 0;
}
//No color, for single agent levels
Agent::Agent(char chr, std::pair<int, int> location):
Entity(chr, location, Entity::BLUE)
{
  this->rank = 0;
}

Agent::Agent(Agent * agt):
Entity(agt->chr, agt->location, agt->color)
{
  this->rank = agt->rank;
}

int Agent::hashCode()
{
  int result = 17;
  result = 31 * result + (int) chr;
  result = 31 * result + rank;
  result = 31 * result + getX();
  result = 31 * result + getY();
  result = 31 * result + color;
  return result;
}

bool Agent::equals(const Agent * agent) const
{
  if (agent == this) return true;
  return agent->chr == chr && agent->rank == rank && agent->color == color && agent->location == location;
}
