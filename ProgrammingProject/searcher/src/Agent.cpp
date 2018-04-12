#include "Agent.h"


Agent::Agent(int num, int rank, std::std::pair<int, int> location, std::string color)
{
    this->num = num;
    this->rank = rank;
    this->location = location;
    this->color = color;
}

Agent::Agent(int num, std::std::pair<int, int> location, String color)
{
    this->num = num;
    this->rank = 0;
    this->location = location;
    this->color = color;
}

Agent::Agent(int num, std::pair<int, int> location)
{
    this->num = num;
    this->rank = 0;
    this->location = location;
    this->color = "";
}

Agent::Agent(Agent * agt)
{
    this->num = agt->num;
    this->rank = 0;
    this->location = agt->location;
    this->color = "";
}

int Agent::hashCode()
{
    int result = 17;
    result = 31 * result + num;
    result = 31 * result + rank;
    result = 31 * result + location.getLeft();
    result = 31 * result + location.getRight();
    result = 31 * result + color.hashCode();
    return result;
}

bool Agent::equals(Agent agent)
{
    if (agent == this) return true;
    /*if (!(o instanceof Agent)) {
        return false;
    }*/

    //Agent agent = (Agent) o;

    return agent.num == num && agent.rank == rank && agent.color.equals(color) && agent.location.equals(location);
}


void Agent::setLocation(std::pair<int, int> location)
{
    if(location.getLeft() == null && location.getRight() == null)
        throw "Cannot be null, Agent.setlocation";
    this->location = location;
}
