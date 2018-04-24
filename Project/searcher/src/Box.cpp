
#include "Box.h"
#include <string>
#include <utility>
#include <boost/pool/object_pool.hpp>

Box::Box(char identifier, std::pair<int, int> location, Entity::COLOR color) :
Entity(identifier, location, color)
{
}

Box::Box(char identifier, std::pair<int, int> location) :
Entity(identifier, location, Entity::BLUE)
{
}

Box::Box(Box * box) :
Entity(box->identifier, box->location, box->color)
{
}

int Box::hashCode()
{
    int result = 17;
    result = 31 * result + (int) (identifier);
    result = 31 * result + std::get<0>(location);
    result = 31 * result + std::get<1>(location);
    result = 31 * result + color;
    return result;
}

bool Box::equals(const Box * box) const
{
    if (box == this) return true;
    return box->identifier == identifier && box->color == color && box->location == location;
}
