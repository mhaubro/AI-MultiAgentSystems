#
#Should include and execute the makefile for the searchclient, as well as maketable,
#prober etc., as well as make clean for all of them, tidying up things
#
#
.PHONY: all clean
#-include $(searchClientDirectory)/Makefile.mk
Root_Directory =  ${strip $(CURDIR)}
searchClientDirectory = $(Root_Directory)/searchClientcpp/src

%.o: %.cpp
	$(CPP) $(CPP_FLAGS) $(INCLUDE_DIRS) -c $< -o $@

OBJECT_DIR = $(Root_Directory)/objects

#Grabs all .cpp in the search client folder
CPP_FILES = $(wildcard $(searchClientDirectory)/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(CPP_FILES))
#%.cpp.o:	%.cpp
#	$(CPP) $(CPP_FLAGS) $(INCLUDE_DIRS) $< -o $@

#OBJ_SC = $(foreach d, $(basename $(CPP_FILES)), $(d).cpp.o)

searchclientbin: $(OBJS)
	$(CPP) -o $@ $^

EXECUTABLES += $(searchClientDirectory)/searchclientbin

#-I marks that headers can be found in a directory
INCLUDE_TEMP += $(Root_Directory)/searchClientcpp/include


all: prober maketable searchclientbin

testCompile:	searchclientbin
	#@echo $(CPP_FILES)
	#@echo $(CPP)
	#@echo $(CPP_FLAGS)
	#@echo $(INCLUDE_DIRS)

default: searchclientbin

prober: $(Root_Directory)prober.cpp
	$(CPP) $(CPP_FLAGS) $< -o prober

maketable: $(Root_Directory)maketable.cpp
	$(CPP) $(CPP_FLAGS) $< -o maketable


CPP = g++
CPP_FLAGS = -std=c++14#Using -O3 to aggressively optimize

#Takes all of include_temp, and puts on -I flag.
INCLUDE_DIRS = $(foreach dir, $(basename $(INCLUDE_TEMP)), -I$(dir))

EXECUTABLES += $(Root_Directory)/prober $(Root_Directory)/maketable $(Root_Directory)/searchclientbin

clean:
	rm $(EXECUTABLES) $(OBJS)




export INCLUDE_DIRS
