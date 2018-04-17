#include <string>
#include <list>
//Returns a list of strings to be printed when printing the total plan.
namespace MasterSearcher{

  bool checkConflicts(){
    return false;
  }

  std::list<std::string> getPlan(Node * initialState){
    int agents = initialState->agents.size();

    std::vector<std::list<Node *>> plans(agents);

    int longestPlan = 0;

    for (int i = 0; i < agents; i++){
      plans[i] = Search(i, initialState);
      if (plans[i].size() > longestPlan){
        longestPlan = plans[i].size();
      }
    }

    std::list<std::string> plan();

    for (int i = 0; i < longestPlan; i++){
      string s = "[";
      for (int j = 0; j < agents-1; j++){
        if (plans[j].size() > j){
          s.append(plans[j][i]);
          s.append(",");
        }
      }
      if (plans[j].size() > j){
        s.append(plans[j][agents-1]);
        s.append("]");
      }
      plan.append(s);
    }
    return plan;
  }
}
