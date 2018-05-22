#ifndef GOAL_H
#define GOAL_H

#include <utility>
#include "Box.h"
#include "Agent.h"
#include <boost/pool/object_pool.hpp>


class Goal : public Entity {
public:

    bool goalState;

    Goal(char chr, Location location);
    Goal(char chr, Location location, Box * box);
};
#endif
