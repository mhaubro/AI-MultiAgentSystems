#
#Should include and execute the makefile for the searchclient, as well as maketable,
#prober etc., as well as make clean for all of them, tidying up things
#
#
all: prober maketable searchclient
testCompile: searchclient

default: all

prober: prober.cpp
	$(CPP) $(CPP_FLAGS) $< -o prober

maketable: maketable.cpp
	$(CPP) $(CPP_FLAGS) $< -o maketable

CPP = g++
CPP_FLAGS = -std=c++14 -O3#Using -O3 to aggressively optimize

Root_Directory =  ${strip $(CURDIR)}
searchClientDirectory = Root_Directory/searchClientcpp

INCLUDE_TEMP =
#Takes all of include_temp, and puts on -I flag.
INCLUDE_DIRS = $(foreach dir, $(basename $(INCLUDE_TEMP)), -I$(dir))

EXECUTABLES += $(Root_Directory)/prober $(Root_Directory)/maketable
OBJECTS = #We can add stuff here to delete on a clean later

clean:
	rm -rf $(EXECUTABLES)


-include $(searchClientDirectory)/Makefile.mk

export INCLUDE_DIRS
