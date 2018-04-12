package searchclient;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;

import static java.lang.Math.abs;

public abstract class Heuristic implements Comparator<Node>
{
    public Heuristic(Node initialState)
	{
		for(Box box : initialState.boxes)
		{
			int minDist = Integer.MAX_VALUE;
			for(Goal goal : Node.goals)
			{
				if(box.chr == Character.toUpperCase(goal.chr) && goal.box == null)
				{
					int dist = manhattan(goal.location, box.location);
					if(dist < minDist)
					{
						goal.box = box;
						minDist = dist;
					}
				}
			}
		}

		for(Agent a : initialState.agents)
		{
			int minDist = Integer.MAX_VALUE;
			for(Goal goal : Node.goals)
			{
				if(goal.agent == null)
				{
					int dist = manhattan(goal.box.location, a.location);
					if(dist < minDist)
					{
						goal.agent = a;
						minDist = dist;
					}
				}
			}
		}
	}

	public int h(Node n)
	{
		int sumDist = 0;
		for(Goal goal : Node.goals)
		{
			sumDist += manhattan(goal.location, goal.box.location);
			sumDist += manhattan(goal.agent.location, goal.box.location);
		}
		return sumDist;
		/**
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
		 */
	}

	public int manhattan(Pair<Integer, Integer> loc1, Pair<Integer, Integer> loc2)
	{
		return abs(loc2.getLeft() - loc1.getLeft()) + abs(loc2.getRight() - loc1.getRight());
	}

	public abstract int f(Node n);

	@Override
	public int compare(Node n1, Node n2) {
		return this.f(n1) - this.f(n2);
	}

	public static class AStar extends Heuristic {
		public AStar(Node initialState) {
			super(initialState);
		}

		@Override
		public int f(Node n) {
			return n.g() + this.h(n);
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
			this.W = W;
		}

		@Override
		public int f(Node n) {
			return n.g() + this.W * this.h(n);
		}

		@Override
		public String toString() {
			return String.format("WA*(%d) evaluation", this.W);
		}
	}

	public static class Greedy extends Heuristic {
		public Greedy(Node initialState) {
			super(initialState);
		}

		@Override
		public int f(Node n) {
			return this.h(n);
		}

		@Override
		public String toString() {
			return "Greedy evaluation";
		}
	}
}
