
#include "Box.h"
#include <string>
#include <utility>

Box::Box(char chr, std::pair<int, int> location, std::string color)
{
    this->chr = chr;
    this->location = location;
    this->color = color;
}

Box::Box(char chr, std::pair<int, int> location)
{
    this->chr = chr;
    this->location = location;
    this->color = "";
}

Box::Box(Box * box){
    this->chr = box->chr;
    this->location = box->location;
    this->color = "";
}

void Box::setLocation(int x, int y)
{
  this->location = std::pair<int, int>(x,y);
}

std::pair<int, int> Box::getLocation(){
  return location;
}

int Box::hashCode()
{
    int result = 17;
    result = 31 * result + (int) (chr);
    result = 31 * result + std::get<0>(location);
    result = 31 * result + std::get<1>(location);
    result = 31 * result + std::hash<std::string>{}(color);
    return result;
}

bool Box::equals(Box * box)
{
    if (box == this) return true;
    return box->chr == chr && box->color == color && box->location == location;
}
int Box::getX(){
  return std::get<0>(location);
}
int Box::getY(){
  return std::get<1>(location);
}
