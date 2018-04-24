#include "Agent.h"
#include "Task.h"
#include <functional>
#include <utility>
#include <boost/pool/object_pool.hpp>
#include "Entity.h"

//boost::object_pool<Agent> Agent::pool;


Agent::Agent(char identifier, int rank, std::pair<int, int> location, COLOR color):
Entity(identifier, location, color){
  this->rank = rank;
}

Agent::Agent(char identifier, std::pair<int, int> location, COLOR color):
Entity(identifier, location, color)
{
    this->rank = 0;
}
//No color, for single agent levels
Agent::Agent(char identifier, std::pair<int, int> location):
Entity(identifier, location, Entity::BLUE)
{
    this->rank = 0;
}

Agent::Agent(Agent * agt):
Entity(agt->identifier, agt->location, agt->color)
{
    this->rank = agt->rank;
}

int Agent::hashCode()
{
    int result = 17;
    result = 31 * result + (int) identifier;
    result = 31 * result + rank;
    result = 31 * result + getX();
    result = 31 * result + getY();
    result = 31 * result + color;
    return result;
}

bool Agent::equals(const Agent * agent) const
{
    if (agent == this) return true;
    return agent->identifier == identifier && agent->rank == rank && agent->color == color && agent->location == location;
}
