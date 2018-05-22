#include "Goal.h"

Goal::Goal(char chr, Location location) :
Entity(chr, location, Entity::BLUE)
{
    this->goalState = false;
}
