#ifndef AGENT_H
#define AGENT_H

#include <tuple>
#include <string>


using namespace std;

class Agent{
    public: 
        tuple<int,int> goal; //Agent Goal position 
        tuple<int, int> position; // Agent Position
        int agentRank; // Agent rank 
        string agentColor; // Color of agent
        Agent();
        Agent(tuple<int, int> goalPos, tuple<int,int> pos, int rank, string color); // Constructor
};
