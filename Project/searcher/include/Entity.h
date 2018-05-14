
#ifndef ENTITY_H
#define ENTITY_H
#include <utility>

/*
Base class for goals, agents and boxes
*/
class Entity {

public:
  enum COLOR {BLUE, RED, GREEN, CYAN, MAGENTA, ORANGE, PINK, YELLOW, NUMCOLS};

  COLOR color;

  Entity(char chr, std::pair<int, int> location, COLOR color);
  //The chr is
  char chr;

  std::pair<int, int> location;

  void setLocation(std::pair<int, int> loc);
  void setDLocation(std::pair<int, int> loc);
  void setLocation(int x, int y);
  void setDLocation(int dx, int dy);
  int getX() const;
  int getY() const;
  std::pair<int, int> getLocation();

};




#endif
