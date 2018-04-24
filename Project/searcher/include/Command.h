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
  static int getdx(int d);
  static int getdy(int d);
  std::string toString();

  enum Type {
    Move, Push, Pull, LASTT
  };

  static std::vector<Command> EVERY;

  Command(int d);
  Command(int t, int d1, int d2);
  Command();
  ~Command();
  int actionType;
	int dirAgent;
	int dirBox;
};


#endif
