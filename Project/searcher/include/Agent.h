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
    int rank;

    std::list<Node *> Search();

    Task * task;

    Agent(char identifier, int rank, std::pair<int, int> location, Entity::COLOR color);
    Agent(char identifier, std::pair<int, int> location, Entity::COLOR color);
    Agent(char identifier, std::pair<int, int> location);
    Agent(Agent * agt);

    int hashCode();

    bool equals(const Agent * o) const;

  private:
    std::pair<int, int> location;

};
#endif
