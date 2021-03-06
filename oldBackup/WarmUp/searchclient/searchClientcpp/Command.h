#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>

class Command{
private:

public:
  enum Dir {
    N, W, E, S, LASTD
  };

  static bool isOpposite(int d1, int d2);
  static int dirToRowChange(int d);
  static int dirToColChange(int d);
  std::string toString();

  enum Type {
    Move, Push, Pull, LASTT
  };

  static std::vector<Command *> EVERY;

  Command(int d);
  Command(int t, int d1, int d2);
  ~Command();

  int actionType;
	int dir1;
	int dir2;

};


#endif
