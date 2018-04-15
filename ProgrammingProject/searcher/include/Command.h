#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>
#include <utility>

class Command{
private:

public:
  enum Dir {
    N, W, E, S, LASTD
  };

  static bool isOpposite(int d1, int d2);
  static int dirToXChange(int d);
  static int dirToYChange(int d);
  std::string to_string();

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
