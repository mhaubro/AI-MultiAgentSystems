#ifndef SEARCHCLIENT_H
#define SEARCHCLIENT_H
#include "centralHeader.h"

#include <sstream>

class SearchClient
{
public:
	SearchClient(std::stringstream serverMessages);
	~SearchClient(Strategy strategy);

	Node initialState;
	
};

#endif
