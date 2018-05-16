#ifndef AGENT_H
#define AGENT_H

#include <utility>
#include <string>
#include <boost/pool/object_pool.hpp>

#include "Task.h"
#include "Node.h"
#include "Plan.h"
#include "Entity.h"
#include "CentralPlanner.h"

class Plan;
class Task;

class Agent : public Entity
{
  public:
	//Is this the number, or an actual rank. I assume actual rank.
    int rank;

    Command * getAction(Node * startstate, Node * temptate);

    Task * currenttask;
    std::list<Node *> search(Node * state);
    Agent(char chr, int rank, std::pair<int, int> location, Entity::COLOR color);
    Agent(char chr, std::pair<int, int> location, Entity::COLOR color);
    Agent(char chr, std::pair<int, int> location);
    Agent(Agent * agt);

    int hashCode();

    bool equals(const Agent * o) const;

private:
    bool workingOnTask = false;//Only set to true, if an agent has started carrying out a task
    int skipNextIte = 0;
    Plan * plan;
    Command * noPlan(Node * startstate);
    void handleConflict();

};
#endif
