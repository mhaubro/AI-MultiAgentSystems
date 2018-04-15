#ifndef BOX_H
#define BOX_H

#include <utility>
#include <string>
#include <boost/pool/object_pool.hpp>


class Box {
public:
    char chr;
    std::pair<int, int> location;
    std::string * color;
    int getX();
    int getY();
    Box(char chr, std::pair<int, int> location, std::string * color);
    Box(char chr, std::pair<int, int> location);
    Box(Box * box);
    void setLocation(int x, int y);
    std::pair<int, int> getLocation();
    int hashCode();
    bool equals(Box * box);
    static boost::object_pool<Box> pool;

  private:

};
#endif
