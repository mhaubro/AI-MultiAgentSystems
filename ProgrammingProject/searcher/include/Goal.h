#ifndef GOAL_H
#define GOAL_H

class Goal {
public:
    char chr;
    std::pair<int, int> location;
    Box * box;
    Agent * agent;
    bool goalState;

    Goal(char chr, std::pair<int, int> location);

    Goal(char chr, std::pair<int, int> location, Box * box);
    void updateBox(Box * box);
    void setAgent(Agent * a);
};
#endif
