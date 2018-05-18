#ifndef BOX_H
#define BOX_H

#include <utility>
#include <string>
#include <boost/pool/object_pool.hpp>
#include "Entity.h"

class Box : public Entity
{
public:
    Box(char chr, std::pair<int, int> location, Entity::COLOR color);
    Box(char chr, std::pair<int, int> location);
    Box(Box * box);
    int hashCode();
    bool equals(const Box * box) const;

    bool workInProgress = false;

  private:

};
#endif
