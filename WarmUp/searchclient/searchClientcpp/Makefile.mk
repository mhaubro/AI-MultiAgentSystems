#To be included by top-level makefile
#Crudely just compiles all .cpp files int this directory into a single executable
#We can go over to compiling .o files and linking afterwards, but this doesn't seem reasonable,
#as everything is C++11


#Also sets an include directory, which can be expanded.
#searchClientDirectory is inherited from makefile above

searchclient: $(CPP_FILES)
	$(CPP) $(CPP_FLAGS) $< $(INCLUDE_DIRS) -o searchclient

#Grabs all .cpp in the search client folder
CPP_FILES = $(wildcard $(searchClientDirectory)/*.cpp)

#-I marks that headers can be found in a directory
INCLUDE_TEMP += $(searchClientDirectory)
