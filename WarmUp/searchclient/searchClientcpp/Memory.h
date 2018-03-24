#ifndef MEMORY_H
#define MEMORY_H
#include "centralHeader.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

int parseLine(char* line){
  // This assumes that a digit will be found and the line ends in " Kb".
  int i = strlen(line);
  const char* p = line;
  while (*p <'0' || *p > '9') p++;
  line[i-3] = '\0';
  i = atoi(p);
  return i;
}

int getValue(){ //Note: this value is in KB!
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
    return result;
}

class Memory {
	private static final double MB = 1024 * 1024;

	public static double used() {
		return getValue()/1024;//Mb




	}

	public static double free() {
	}

	public static double total() {
		return RUNTIME.totalMemory() / MB;
	}

	public static double max() {
		return RUNTIME.maxMemory() / MB;
	}

	public static String stringRep() {
		return String.format("[Used: %.2f MB, Free: %.2f MB, Alloc: %.2f MB, MaxAlloc: %.2f MB]", used(), free(), total(), max());
	}
}

#endif
