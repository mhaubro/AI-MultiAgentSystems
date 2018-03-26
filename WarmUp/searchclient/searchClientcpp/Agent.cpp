#include <tuple>
#include <string>
#include "Agent.h"

using namespace std;

Agent::Agent(){
    goal = make_tuple(0,0);
    position = make_tuple(0,0);
    agentRank = 0;
    agentColor = "red";
}

Agent::Agent(tuple<int, int> goalPos, tuple<int,int> pos, int rank, string color){
    goal = goalPos;
    position = pos;
    agentRank = rank;
    agentColor = color; 
}




