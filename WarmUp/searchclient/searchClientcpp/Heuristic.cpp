/*
#include "Command.h"
#include "centralHeader.h"

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;

import static java.lang.Math.abs;

public abstract class Heuristic implements Comparator<Node> {
    public HashMap<Character, ArrayList<Pair<Integer, Integer>>> goalsMap;

    public Heuristic(Node initialState) {
    	goalsMap = new HashMap<>();
        for(int i = 0; i < Node.MAX_ROW; i++)
        {
            for(int j = 0; j < Node.MAX_COL; j++)
            {
                char chr = Node.goals[i][j];
                if('a' <= chr && chr <= 'z')
                {
                    if(goalsMap.containsKey(chr))
                        goalsMap.get(chr).add(new Pair(i,j));
                    else
                    {
                        goalsMap.put(chr, new ArrayList<>());
                        goalsMap.get(chr).add(new Pair(i,j));
                    }
                }
            }
        }
		// Here's a chance to pre-process the static parts of the level.
	}

	public int h(Node n) {
	    int minBDist = Integer.MAX_VALUE;
	    int minADist = Integer.MAX_VALUE;
        int sumDist = 0;
        for(int i = 0; i < Node.MAX_ROW; i++)
        {
            for(int j = 0; j < Node.MAX_COL; j++)
            {
               char chr = n.boxes[i][j];
               if('A' <= chr && chr <= 'Z')
               {
                       minBDist = Integer.MAX_VALUE;
                       int aDist = abs(i - n.agentRow) + abs(j - n.agentCol);
                       minADist = (aDist < minADist) ? aDist : minADist;
                       ArrayList<Pair<Integer, Integer>> chrGoals = goalsMap.get(Character.toLowerCase(chr));
                       for(Pair goal : chrGoals)
                       {
                           //if(n.boxes[i][j] == Node.goals[(int) goal.getLeft()][(int) goal.getRight()])
                           int dist = abs(i - (int) goal.getLeft()) + abs(j - (int) goal.getRight());
                           minBDist = (dist < minBDist) ? dist : minBDist;
                           // minDist = dist < minDist ? dist : minDist;
                       }
                       sumDist += minBDist;
            	}
        	}
    	}
        return sumDist;
	}

	public abstract int f(Node n);

	@Override
	public int compare(Node n1, Node n2) {
		return this->f(n1) - this->f(n2);
	}

	public static class AStar extends Heuristic {
		public AStar(Node initialState) {
			super(initialState);
		}

		@Override
		public int f(Node n) {
			return n.g() + this->h(n);
		}

		@Override
		public String toString() {
			return "A* evaluation";
		}
	}

	public static class WeightedAStar extends Heuristic {
		private int W;

		public WeightedAStar(Node initialState, int W) {
			super(initialState);
			this->W = W;
		}

		@Override
		public int f(Node n) {
			return n.g() + this->W * this->h(n);
		}

		@Override
		public String toString() {
			return String.format("WA*(%d) evaluation", this->W);
		}
	}

	public static class Greedy extends Heuristic {
		public Greedy(Node initialState) {
			super(initialState);
		}

		@Override
		public int f(Node n) {
			return this->h(n);
		}

		@Override
		public String toString() {
			return "Greedy evaluation";
		}
	}
}
*/
