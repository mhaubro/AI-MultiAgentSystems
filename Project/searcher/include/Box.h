#ifndef BOX_H
#define BOX_H

#include <utility>
#include <string>
#include <boost/pool/object_pool.hpp>
#include "Entity.h"

class Box : public Entity
{
public:
    Box(char chr, Location location, Entity::COLOR color);
    Box(char chr, Location location);
    Box(Box * box);
    int hashCode();
    bool equals(const Box * box) const;

    bool workInProgress = false;

  private:

};
#endif
