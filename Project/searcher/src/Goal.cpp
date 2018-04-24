#include "Goal.h"

Goal::Goal(char chr, std::pair<int, int> location) :
Entity(chr, location, Entity::BLUE)
{
    this->goalState = false;
}
