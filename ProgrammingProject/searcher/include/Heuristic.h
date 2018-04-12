
//Not in use as of right now
#ifndef HEURISTIC_H
#define HEURISTIC_H

class Heuristic implements Comparator<Node>
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


#endif
