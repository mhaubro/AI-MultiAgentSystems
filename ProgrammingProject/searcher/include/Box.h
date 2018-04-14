#ifndef BOX_H
#define BOX_H

#include <utility>
#include <string>


class Box {
public:
    char chr;
    std::pair<int, int> location;
    std::string color;
    int getX();
    int getY();
    Box(char chr, std::pair<int, int> location, std::string color);
    Box(char chr, std::pair<int, int> location);
    Box(Box * box);
    void setLocation(std::pair<int, int> location);
    int hashCode();
    bool equals(Box * box);
};
#endif
