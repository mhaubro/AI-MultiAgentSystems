#
#Should include and execute the makefile for the searchclient, as well as maketable,
#prober etc., as well as make clean for all of them, tidying up things
#
#
.PHONY: all clean
#-include $(SOURCEDIR)/Makefile.mk
Root_Directory =  ${strip $(CURDIR)}
SCDIR = $(Root_Directory)/searchClientcpp
SOURCEDIR = $(SCDIR)/src

OBJECT_DIR = $(Root_Directory)/objects

#OBJ_FOLDERS = $(sort $(dir $(wildcard $(SCDIR)/*/)))

#directories: ${OBJ_FOLDERS}

#${OBJ_FOLDERS}:
#	mkdir -p ${OBJ_FOLDERS}


$(OBJECT_DIR)/%.cpp.o: $(SCDIR)/%.cpp
	$(CPP) $(CPP_FLAGS) $(INCLUDE_DIRS) -c $< -o $@


#Grabs all .cpp in the search client folder
CPP_FILES = $(wildcard $(SOURCEDIR)/*.cpp)
CPPSHORT = $(subst $(SCDIR),,$(CPP_FILES))
OBJS = $(foreach d, $(CPPSHORT), $(OBJECT_DIR)$(d).o)
#%.cpp.o:	%.cpp
#	$(CPP) $(CPP_FLAGS) $(INCLUDE_DIRS) $< -o $@

#OBJ_SC = $(foreach d, $(basename $(CPP_FILES)), $(d).cpp.o)

searchclientbin: $(OBJS)
	$(CPP) -o $@ $^

EXECUTABLES += $(Root_Directory)/searchclientbin

#-I marks that headers can be found in a directory
INCLUDE_TEMP += $(Root_Directory)/searchClientcpp/include

all: prober maketable searchclientbin

testCompile:
	@echo $(CPP_FILES)
	@echo $(CPPSHORT)
	@echo $(OBJ_FOLDERS)

default: searchclientbin

prober: $(Root_Directory)prober.cpp
	$(CPP) $(CPP_FLAGS) $< -o prober

maketable: $(Root_Directory)maketable.cpp
	$(CPP) $(CPP_FLAGS) $< -o maketable


CPP = g++
CPP_FLAGS = -std=c++14#Using -O3 to aggressively optimize

#Takes all of include_temp, and puts on -I flag.
INCLUDE_DIRS = $(foreach dir, $(basename $(INCLUDE_TEMP)), -I$(dir))

#EXECUTABLES += $(Root_Directory)/prober $(Root_Directory)/maketable $(Root_Directory)/searchclientbin

clean:
	rm $(EXECUTABLES) $(OBJS)




export INCLUDE_DIRS
