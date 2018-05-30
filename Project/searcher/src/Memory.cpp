#include "Memory.h"
#include <string>
#include <cstring>
#include "iostream"

	int Memory::limit;

	int Memory::parseLine(char* line){
		// This assumes that a digit will be found and the line ends in " Kb".
		int i = strlen(line);
		const char* p = line;
		while (*p <'0' || *p > '9') p++;
		line[i-3] = '\0';
		i = atoi(p);
		return i;
	}


	int Memory::getValue(){ //Note: this value is in MB!
		FILE* file = fopen("/proc/self/status", "r");
		int result = -1;
		char line[128];

		while (fgets(line, 128, file) != NULL){
			if (strncmp(line, "VmSize:", 7) == 0){
				result = parseLine(line);
				break;
			}
		}
		fclose(file);
		return result/1024;
	}

	bool Memory::checkMemory(){
		if (used() > limit){
			//////std::cerr << "\nFailed memory check\n\n";
			throw "Out of memory";
			return false;
		}
		return true;
	}

	int Memory::used() {
		return getValue();
	}

	int Memory::free() {
		return (limit - getValue());
	}

	int Memory::max() {
		return limit;
	}

	std::string Memory::stringRep() {
		checkMemory();
		//Inspired by https://stackoverflow.com/questions/25169915/is-writing-to-str0-buffer-of-a-stdstring-well-defined-behaviour-in-c11
		char stemp[100];
		sprintf(stemp, "UsedM: %d MB, Free: %d MB, Max: %d MB", used(), free(), max());
		std::string s;
		s.resize(sizeof(stemp)-1);
		memcpy(&s[0], stemp, strlen(stemp)-1);
		return s;
	}
