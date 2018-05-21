#include <list>
#include <string>
#include "Node.h"
#include <vector>

namespace Initializer{

  std::list<std::string> readInput();
  Node * storeInput();
  std::vector<Node *> setupEnvironment();
}
