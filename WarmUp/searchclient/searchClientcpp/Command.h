#ifndef COMMAND_H
#define COMMAND_H
#include "centralHeader.h"


class Command{
private:



protected:


public:
  enum Dir {
    N, W, E, S
  };

  enum Type {
    Move, Push, Pull
  };

  static final std::vector<Command> EVERY;


}


#endif
