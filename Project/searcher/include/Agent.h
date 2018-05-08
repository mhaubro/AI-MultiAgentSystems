#ifndef AGENT_H
#define AGENT_H

#include <utility>
#include <string>
#include <boost/pool/object_pool.hpp>

#include "Task.h"
#include "Node.h"
#include "Entity.h"
class Task;
class Node;
class Agent : public Entity
{
  public:
	//Is this the number, or an actual rank. I assume actual rank.
    int rank;

    //std::list<Node *> Search();

    Command * getAction(Node * startstate, Node * temptate);

    Task * task;
    std::list<Node *> search(Node * state);
    Agent(char chr, int rank, std::pair<int, int> location, Entity::COLOR color);
    Agent(char chr, std::pair<int, int> location, Entity::COLOR color);
    Agent(char chr, std::pair<int, int> location);
    Agent(Agent * agt);

    int hashCode();

    bool equals(const Agent * o) const;


};
#endif
