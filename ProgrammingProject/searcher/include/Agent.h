#ifndef AGENT_H
#define AGENT_H


#include <string>
#include <std::pair>

class Agent
{
public:

    int num;
    int rank;
    std::std::pair<int, int> location;
    std::string color;

    Agent(int num, int rank, std::std::pair<int, int> location, std::string color);
    Agent(int num, std::std::pair<int, int> location, String color);
    Agent(int num, std::pair<int, int> location);
    Agent(Agent * agt);

    int hashCode();
    bool equals(Object o);
    void setLocation(std::pair<int, int> location);
}
#endif
