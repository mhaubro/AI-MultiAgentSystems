#ifndef BOXTOGOALTASK_H
#define BOXTOGOALTASK_H
#include "Goal.h"
#include "Box.h"
#include "MoveBoxTask.h"
class BoxToGoalTask : public MoveBoxTask{

  Goal * goal;

  BoxToGoalTask(Box * box, Goal * goal);
};


#endif
