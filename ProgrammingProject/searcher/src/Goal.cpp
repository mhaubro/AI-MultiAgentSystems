#include "Goal.h"

Goal::Goal(char chr, std::pair<int, int> location)
{
    this->chr = chr;
    this->location = location;
    this->box = NULL;
    this->goalState = false;
}

Goal::Goal(char chr, std::pair<int, int> location, Box * box)
{
    this->chr = chr;
    this->location = location;
    this->box = box;
}

std::pair<int, int> Goal::getLocation(){
  return location;
}

void Goal::updateBox(Box * box)
{
    this->box = box;
}

void Goal::setAgent(Agent * a)
{
    this->agent = a;
}
int Goal::getX(){
  return std::get<0>(location);
}
int Goal::getY(){
  return std::get<1>(location);
}
