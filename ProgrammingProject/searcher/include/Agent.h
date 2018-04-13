#ifndef AGENT_H
#define AGENT_H

#include <utility>
#include <string>

class Agent
{
public:

    int num;
    int rank;
    std::pair<int, int> location;
    std::string color;

    Agent(int num, int rank, std::pair<int, int> location, std::string color);
    Agent(int num, std::pair<int, int> location, std::string color);
    Agent(int num, std::pair<int, int> location);
    Agent(Agent * agt);

    int hashCode();
    bool equals(Agent * o);
    void setLocation(std::pair<int, int> location);
};
#endif
