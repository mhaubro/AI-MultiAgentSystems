#ifndef GOAL_H
#define GOAL_H

#include <utility>
#include "Box.h"
#include "Agent.h"
#include <boost/pool/object_pool.hpp>


class Goal {
public:
    char chr;
    std::pair<int, int> location;
    Box * box;
    Agent * agent;
    
    bool goalState;
    int getX();
    int getY();
    std::pair<int, int> getLocation();

    Goal(char chr, std::pair<int, int> location);
    Goal(char chr, std::pair<int, int> location, Box * box);
    void updateBox(Box * box);
    void setAgent(Agent * a);
};
#endif
