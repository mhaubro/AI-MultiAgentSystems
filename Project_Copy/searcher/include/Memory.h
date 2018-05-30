#ifndef MEMORY_H
#define MEMORY_H
//#include "centralHeader.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <string>

class Memory {
	private:
		static int parseLine(char* line);
		static int getValue();

	public:
		static int limit;
		static int used();
		static int free();
		static int total();
		static int max();
		static std::string stringRep();
		static bool checkMemory();
};

#endif
