#include <string>
#include <list>
#include <iostream>
#include "Agent.h"
#include "MasterSearcher.h"
#include "Node.h"
//#include ""
//Returns a list of strings to be printed when printing the total plan.
namespace MasterSearcher{

  bool checkConflicts(){
    return false;
  }

  std::vector<std::string> getPlan(Node * initialState){

    int agents = initialState->agents.size();
    std::cerr << "Agents: " << agents;

    std::vector<std::list<Node *>> plans(agents);

    int longestPlan = 0;
    Node * state = initialState;
    //while (true){

      /*Iteratively searches for a solutions*/
      /*Do this over and over again*/
      /* Start over whenever there's a conflict*/
      /*Stores these plans*/

      for (int i = 0; i < agents; i++){
        plans[i] = state->agents[i].search( state);
        if (plans[i].size() > longestPlan){
          longestPlan = plans[i].size();
        }
      }

      std::list<Node *> aOnePlan = plans[1];
      std::cerr << "Printing plan for agent 0\n";
      std::cerr << "Plan size: " << aOnePlan.size() << "\n";

      int s = aOnePlan.size();
      for(int i = 0; i < s; i++){
        std::cerr << aOnePlan.front()->toString();
        aOnePlan.pop_front();
      }
      /*Creates a list of strings that is the actual plan*/
      /*Keeps checking for conflicts*/
      std::vector<std::string> plan = std::vector<std::string>();

      bool isPlanning = true;
      for (int i = 0; i < longestPlan; i++){
        std::string s = "[";
        for (int j = 0; j < agents-1; j++){
          if (plans[j].size() > i){
            s.append("NoOp");
            s.append(",");
          }
          /*Checks if we're planning or if there's been a conflict, and if the next move is okay*/
          else if (state->checkAndChangeState(j, plans[j].front()->action) && isPlanning){
            /*If the plans are okay, no conflict */
            s.append(plans[j].front()->action->toString());
            s.append(",");
            plans[j].pop_front();
          } else {
            isPlanning = false;
            /*Do replanning*/
            s.append("NoOp");
            s.append(",");
          }

        }
        /*Taking the last agent, fencepost type problem*/
        if (plans[agents-1].size() > i){
          s.append("NoOp");
          s.append("]");
        }
        else if ( state->checkAndChangeState(agents-1, plans[agents-1].front()->action) && isPlanning){
          //Handles fencepost, as only the elements must be ,-separated
          s.append(plans[agents-1].front()->action->toString());
          s.append("]");
          plans[agents-1].pop_front();
        } else {
          isPlanning = false;
          s.append("NoOp");
          s.append("]");
        }
        plan.push_back(s);
        std::cerr << "Iteration\n";
        if (state->isGoalState()){
          return plan;
        }
        if (!isPlanning){
          break;
        }
      //}
    }
  }
}
