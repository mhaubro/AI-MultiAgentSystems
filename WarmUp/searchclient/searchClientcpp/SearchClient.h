#ifndef SEARCHCLIENT_H
#define SEARCHCLIENT_H
#include "centralHeader.h"

#include <iostream>

class SearchClient
{
public:
	SearchClient(std::istream serverMessages);
	~SearchClient();

	Node initialState;

};

#endif
