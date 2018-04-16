#include "BoxToGoalTask.h"

BoxToGoalTask::BoxToGoalTask(Box * box, Goal * goal) : MoveBoxTask(box){
    this->goal = goal;
}
