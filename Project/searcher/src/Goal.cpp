#include "Goal.h"

Goal::Goal(char chr, Location location, int region, int id) :
Entity(chr, location, Entity::BLUE, region, id)
{
    this->goalState = false;
}
