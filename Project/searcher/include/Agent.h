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
#include "RequestFreeSpaceTask.h"

class Plan;
class Task;
class RequestFreeSpaceTask;

class Agent : public Entity
{
  public:
	//Is this the number, or an actual rank. I assume actual rank.
    int rank;

    Command * getAction(Node * startstate, Node * temptate);

    Task * task;
    std::list<Node *> search(Node * state);
    std::list<Node *> Nakedsearch(Node * state);
    std::list<Node *> noBoxesOrAgents(Node * state, Box * box);

    Agent(char chr, int rank, Location location, Entity::COLOR color);
    Agent(char chr, Location location, Entity::COLOR color);
    Agent(char chr, Location location);
    Agent(const Agent * agt);

    int hashCode();

    bool equals(const Agent * o) const;

private:
    RequestFreeSpaceTask * t;//Can at max request one at a time
    bool workingOnTask = false;//Only set to true, if an agent has started carrying out a task
    int skipNextIte = 0;
    Plan * plan;
    Command * noPlan(Node * startstate, Node * tempstate);
    Command * handleConflict();

};
#endif
