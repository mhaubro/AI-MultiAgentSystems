#ifndef SEARCHCLIENT_H
#define SEARCHCLIENT_H
#include "Node.h"
#include "Strategy.h"

#include <iostream>
#include <list>

class SearchClient
{
public:
	SearchClient();
	~SearchClient();

	std::list<Node *> search(Strategy * strategy);

	Node * initialState;

};

#endif
