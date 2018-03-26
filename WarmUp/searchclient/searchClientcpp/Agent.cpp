#include <tuple>
#include <string>

using namespace std;

class Agent{
    public: 
        tuple<int,int> goal; //Agent Goal position 
        tuple<int, int> position; // Agent Position
        int agentRank; // Agent rank 
        string color; // Color of agent
        Agent(); // Constructor
};



