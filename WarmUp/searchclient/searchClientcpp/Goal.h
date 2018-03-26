#ifndef GOAL_H
#define GOAL_H

#include "Goal.h"
#include "centralHeader.h"
#include <tuple>
#include <string>


class Goal {
public:
    char Gchr;
    std::string Gcolor;
    std::tuple<int, int> Glocation;
    Goal(char chr, std::string color, std::tuple<int, int> location);
};

#endif