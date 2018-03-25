#include "Memory.h"
#include "centralHeader.h"

namespace Memory{


	int parseLine(char* line){
		// This assumes that a digit will be found and the line ends in " Kb".
		int i = strlen(line);
		const char* p = line;
		while (*p <'0' || *p > '9') p++;
		line[i-3] = '\0';
		i = atoi(p);
		return i;
	}


	int getValue(){ //Note: this value is in MB!
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

	bool checkMemory(){
		if (used() > limit){
			//Throw exception?
			return false;
		}
		return true;
	}

	static double used() {
		return (getValue) ;
	}

	static double free() {
		return (limit - getValue());
	}

	static double max() {
		return limit;
	}

	static String stringRep() {
		return String.format("[Used: %d MB, Free: %d MB, MaxAlloc: %d MB]", used(), free(), max());
	}
}
