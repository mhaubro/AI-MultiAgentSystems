#include "goal.h"

Goal::Goal(char chr, std::pair<int, int> location)
{
    this->chr = chr;
    this->location = location;
    this->box = null;
    this->goalState = false;
}

Goal::Goal(char chr, std::pair<int, int> location, Box * box)
{
    this->chr = chr;
    this->location = location;
    this->box = box;
}

void Goal::updateBox(Box * box)
{
    this->box = box;
}

void Goal::setAgent(Agent * a)
{
    this->agent = a;
}
