#ifndef STRATEGY_H
#define STRATEGY_H
#include <list>

// predefines
class Node;
class Agent;
class Task;

std::list<Node *> a_star_search(Node * state, Agent* agent, Task* task, int ite);

#endif
