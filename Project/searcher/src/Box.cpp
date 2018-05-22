
#include "Box.h"
#include <string>
#include <utility>
#include <boost/pool/object_pool.hpp>

Box::Box(char chr, Location location, Entity::COLOR color, int region) :
Entity(chr, location, color, region)
{
}

Box::Box(char chr, Location location, int region) :
Entity(chr, location, Entity::BLUE, region)
{
}

Box::Box(Box * box) :
Entity(box->getChar(), box->getLocation(), box->getColor(), box->getRegion())
{
}

int Box::hashCode()
{
    int result = 17;
    result = 31 * result + (int) (chr);
    result = 31 * result + location.getX();
    result = 31 * result + location.getY();
    result = 31 * result + color;
    return result;
}

bool Box::equals(const Box * box) const
{
    if (box == this) return true;
    return box->chr == chr && box->color == color && box->location == location;
}
