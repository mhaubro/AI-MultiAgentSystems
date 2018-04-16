#include "Goal.h"
#include <tuple>
#include <string>

Goal::Goal(char chr, std::tuple<int, int> location){
    Gchr = chr;
    Glocation = location;
}

Goal::Goal(char chr, std::string color, std::tuple<int, int> location){
    Gchr = chr;
    Gcolor = color;
    Glocation = location;
}
