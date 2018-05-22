#include "Goal.h"

Goal::Goal(char chr, Location location, int region) :
Entity(chr, location, Entity::BLUE, region)
{
    this->goalState = false;
}
