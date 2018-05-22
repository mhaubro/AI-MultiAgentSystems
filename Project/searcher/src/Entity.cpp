#include "Entity.h"
#include <iostream>
#include <string>

Entity::Entity(char chr, Location location, COLOR color) :
chr(chr), location(location), color(color) {

}

Entity::COLOR Entity::getColor() const{
	return color;
}
char Entity::getChar() const{
	return chr;
}

double Entity::getDistance(Entity e) const{
	return location.getDistance(e.getLocation());
}

int Entity::getManhattan(Entity e) const{
	return location.getManhattan(e.getLocation());
}

void Entity::setLocation(Location loc){
	location = loc;
}
void Entity::setDLocation(Location loc){
	//std::cerr << "delta: " << loc.toString() << "current "<< location.toString() << "\n";
	location += loc;
	//std::cerr << "current "<< location.toString() << "\n";
}

Location Entity::getLocation() const
{
	return location;
}
