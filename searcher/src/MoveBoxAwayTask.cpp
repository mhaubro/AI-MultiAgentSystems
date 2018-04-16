#include "MoveBoxAwayTask.h"


MoveBoxAwayTask::MoveBoxAwayTask(Box * box, std::pair<int, int> loc) : MoveBoxTask(box) {
      this->boxDestination = loc;
}
