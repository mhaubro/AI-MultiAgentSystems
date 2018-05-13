#include "Node.h"
#include "CentralPlanner.h"
#include <list>
#include <string>

namespace MasterSearcher{
  std::vector<std::string> getPlan(Node * initialState);
  bool checkConflicts();
  std::vector<Node *> Search(int i, Node * state);
  void printStep(std::string s);
}
