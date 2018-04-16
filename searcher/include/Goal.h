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

    bool goalState;
    int getX() const;
    int getY() const;
    std::pair<int, int> getLocation();

    Goal(char chr, std::pair<int, int> location);
    Goal(char chr, std::pair<int, int> location, Box * box);
};
#endif
