
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

void Box::setLocation(std::pair<int, int> location)
{
    this->location = location;
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
