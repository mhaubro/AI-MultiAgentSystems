#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>
#include "centralHeader.h"

class Command{
private:

public:
  enum Dir {
    N=0, W=1, E=2, S=3, LASTD=4
  };

  static bool isOpposite(int d1, int d2);
  static int dirToRowChange(int d);
  static int dirToColChange(int d);
  std::string to_string();

  enum Type {
    Move=0, Push=1, Pull=2, LASTT=3
  };

  static std::vector<Command> EVERY;

  Command(int d);
  Command(int t, int d1, int d2);
  ~Command();

protected:

  int actionType;
	int dir1;
	int dir2;
};


#endif
