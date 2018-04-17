#ifndef AGENT_H
#define AGENT_H

#include <utility>
#include <string>
#include <boost/pool/object_pool.hpp>

#include "Task.h"

class Task;

class Agent
{
  public:
    int num;
    int rank;
    std::pair<int, int> location;
    std::string * color;

    Task * task;
    int getX() const;
    int getY() const;

    Agent(int num, int rank, std::pair<int, int> location, std::string * color);
    Agent(int num, std::pair<int, int> location, std::string * color);
    Agent(int num, std::pair<int, int> location);
    Agent(Agent * agt);

    int hashCode();
    bool equals(const Agent * o) const;
    void setLocation(int x, int y);
    std::pair<int, int> getLocation();
    static boost::object_pool<Agent> pool;

  private:

};
#endif
