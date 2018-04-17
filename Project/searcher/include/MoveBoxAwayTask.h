#ifndef MOVEBOXAWAYTASK_H
#define MOVEBOXAWAYTASK_H
#include "Box.h"
#include "MoveBoxTask.h"
#include <utility>

class MoveBoxAwayTask : public MoveBoxTask {

  std::pair<int, int> boxDestination;
public:
  MoveBoxAwayTask(Box * box, std::pair<int, int> loc);
};



#endif
