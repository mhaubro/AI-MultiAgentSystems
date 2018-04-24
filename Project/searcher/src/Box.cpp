
#include "Box.h"
#include <string>
#include <utility>
#include <boost/pool/object_pool.hpp>

Box::Box(char chr, std::pair<int, int> location, Entity::COLOR color) :
Entity(chr, location, color)
{
}

Box::Box(char chr, std::pair<int, int> location) :
Entity(chr, location, Entity::BLUE)
{
}

Box::Box(Box * box) :
Entity(box->chr, box->location, box->color)
{
}

int Box::hashCode()
{
    int result = 17;
    result = 31 * result + (int) (chr);
    result = 31 * result + std::get<0>(location);
    result = 31 * result + std::get<1>(location);
    result = 31 * result + color;
    return result;
}

bool Box::equals(const Box * box) const
{
    if (box == this) return true;
    return box->chr == chr && box->color == color && box->location == location;
}
