#include "Goal.h"

Goal::Goal(char chr, std::pair<int, int> location)
{
    this->chr = chr;
    this->location = location;
    this->goalState = false;
}

std::pair<int, int> Goal::getLocation(){
  return location;
}



int Goal::getX() const{
  return std::get<0>(location);
}
int Goal::getY() const{
  return std::get<1>(location);
}
