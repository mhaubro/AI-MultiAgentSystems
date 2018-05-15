#include "Heuristic.h"
#include <locale>
#include <string>
#include <sstream>
#define INTEGER_MAX 0x7FFFFFFF//31 1-bits is max due to sign.

  Heuristic::~Heuristic(){

  }

  Heuristic::Heuristic(Node * initialState)
	{
	/*	for(auto & box : initialState->boxes)
		{
			int minDist = INTEGER_MAX;
			for(auto & goal : Node::goals)
			{
				if(box.chr == std::toupper(goal.chr))// && goal->box == NULL)
				{
					int dist = manhattan(goal.location, box.location);
					if(dist < minDist)
					{
						//goal->box = box;
						minDist = dist;
					}
				}
			}
		}

		for(auto & a : initialState->agents)
		{
			int minDist = INTEGER_MAX;
			for(auto & goal : Node::goals)
			{
				/*if(goal->agent == NULL)
				{
					int dist = manhattan(goal->box->location, a->location);
					if(dist < minDist)
					{
						goal->agent = a;
						minDist = dist;
					}
				}
			}
		}*/
	}

	int Heuristic::h(Node * n)
	{
		int sumDist = 0;
		for(auto & goal : Node::goals)
		{
			//sumDist += manhattan(goal->location, goal->box->location);
			//sumDist += manhattan(goal->agent->location, goal->box->location);
		}
		return sumDist;
		/**
	    int minBDist = INTEGER_MAX;
	    int minADist = INTEGER_MAX;
        int sumDist = 0;
        for(int i = 0; i < Node.MAX_ROW; i++)
        {
            for(int j = 0; j < Node.MAX_COL; j++)
            {
            	char chr = n.boxes[i][j];
               	if('A' <= chr && chr <= 'Z')
               	{
                	minBDist = INTEGER_MAX;
                    int aDist = abs(i - n.agentRow) + abs(j - n.agentCol);
                    minADist = (aDist < minADist) ? aDist : minADist;
                    ArrayList<std::pair<int, int>> chrGoals = goalsMap.get(Character.toLowerCase(chr));
                    for(std::pair goal : chrGoals)
                    {
                    	//if(n.boxes[i][j] == Node.goals[(int) goal->getX()][(int) goal->getY()])
                        int dist = abs(i - (int) goal->getX()) + abs(j - (int) goal->getY());
                        minBDist = (dist < minBDist) ? dist : minBDist;
                        // minDist = dist < minDist ? dist : minDist;
					}
                    sumDist += minBDist;
            	}
        	}
    	}
        return sumDist;
		 */
	}

	int Heuristic::manhattan(std::pair<int, int> loc1, std::pair<int, int> loc2)
	{
		return abs(std::get<0>(loc2) - std::get<0>(loc1)) + abs(std::get<1>(loc2) - std::get<1>(loc1));
	}

  int Heuristic::f(Node * n){
    return 0;
  }


/*	int Heuristic::compare(Node * n1, Node * n2) {
		return this->f(n1) - this->f(n2);
	}*/


AStar::AStar(Node * initialState) : Heuristic(initialState) {
}


		int AStar::f(Node * n) {
			return n->g() + this->h(n);
		}


		std::string AStar::toString() {
			return "A* evaluation";
		}



	 WeightedAStar::WeightedAStar(Node *initialState, int W) : Heuristic(initialState){
			this->W = W;
		}


		int WeightedAStar::f(Node *n) {
			return n->g() + this->W * this->h(n);
		}


		std::string WeightedAStar::toString() {
      std::ostringstream os;
      os << "WA*(" << this->W <<") evaluation\n";
			return os.str();
		}


		Greedy::Greedy(Node * initialState) : Heuristic(initialState){
		}


		int Greedy::f(Node * n) {
			return this->h(n);
		}


		std::string Greedy::toString() {
			return "Greedy evaluation";
		}
