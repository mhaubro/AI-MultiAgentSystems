#include "Entity.h"

Entity::Entity(char identifier, std::pair<int, int> location, COLOR color) :
identifier(identifier), location(location), color(color) {

}


void Entity::setLocation(int x, int y)
{
  this->location = std::pair<int, int>(x,y);
}

void Entity::setDLocation(int dx, int dy)
{
  setLocation(dx + this->getX(), dy + this->getY());
}

int Entity::getX() const
{
  return std::get<0>(location);
}

int Entity::getY() const
{
  return std::get<1>(location);
}

std::pair<int, int> Entity::getLocation()
{
  return location;
}
