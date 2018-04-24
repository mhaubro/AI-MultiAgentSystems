#ifndef STRATEGY_H
#define STRATEGY_H
#include <string>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <functional>
#include <unordered_set>
#include "Memory.h"
#include "Node.h"
#include "Command.h"


template<class T>
class Strategy	
{
public:
	Strategy() = 0;
	~Strategy() = 0;

	virtual queue<Command> search(Node& StartNode, Agent& agent, Task& task);
}

class BFS	
{
public:
	BFS() = 0;
	~BFS() = 0;

	queue<Command> search(Node& StartNode, Agent& agent, Task& task);
}



Class



#endif
