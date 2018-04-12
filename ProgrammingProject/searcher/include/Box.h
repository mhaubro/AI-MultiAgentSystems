#ifndef BOX_H
#define BOX_H
class Box {
public:
    char chr;
    std::pair<int, int> location;
    std::string color;

    Box(char chr, std::pair<int, int> location, std::string color);
    Box(char chr, std::pair<int, int> location);
    Box(Box * box);
    void setLocation(std::pair<int, int> location);
    int hashCode();
    bool equals(Box box);
}
#endif
