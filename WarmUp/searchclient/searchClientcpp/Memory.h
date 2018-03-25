#ifndef MEMORY_H
#define MEMORY_H
#include "centralHeader.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

public class Memory {
private:
	static final int limit;
  int parseLine(char* line)
  int getValue();

public:
	static double used();
	static double free();
	static double total();
	static double max();
	static String stringRep();
  bool checkMemory();
}

#endif
