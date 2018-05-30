#ifndef GOAL_H
#define GOAL_H

#include <utility>
#include "Box.h"
#include "Agent.h"
#include <boost/pool/object_pool.hpp>
#include <unordered_set>


class Goal : public Entity {
public:

    bool goalState;

    std::vector<Goal *> predecessors;

    Goal(char chr, Location location, int region, int id);
    Goal(char chr, Location location, Box * box, int region, int id);
};
#endif
