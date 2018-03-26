#include "Goal.h"
#include "centralHeader.h"
#include <tuple>
#include <string>

Goal::Goal(char chr, std::string color, std::tuple<int, int> location){
    Gchr = chr;
    Gcolor = color;
    Glocation = location;
}