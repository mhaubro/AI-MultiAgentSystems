package searchclient;

import java.util.Comparator;
import java.awt.Point;
import java.util.ArrayList;
import searchclient.NotImplementedException;

public abstract class Heuristic implements Comparator<Node> {
	
	int numberOfGoals;
	ArrayList<Point> goals;
	
	
	public Heuristic(Node initialState) {
		goals = new ArrayList<Point>();
		// Here's a chance to pre-process the static parts of the level.
		for (int i = 0; i < initialState.goals.length; i++) {
			for (int j = 0; j < initialState.goals[i].length; j++) {
				if (initialState.goals[i][j]=='a') {
					Point p = new Point(i,j);
					goals.add(p);
				}
			}
		}
		if (goals.size() == 0) {
			System.err.println("ERROR");
		}
	}

	public int h(Node n) {
		throw new NotImplementedException();
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
		private int calcScore(int i, int j, Node n) {//Calculates distance between a box and a goal
			int bestScore = 0xffffff;//Max int
			for (int s = 0; s < goals.size(); s++) {
				Point p = goals.get(s);
				int score = Math.abs(i-p.x) + Math.abs(j-p.y);
				if (score < bestScore) {
					bestScore = score;
				}
			}
			bestScore += Math.abs(i-n.agentRow) + Math.abs(j-n.agentCol);
			return bestScore;
		}

		@Override
		public int f(Node n) {
			return n.g() + this.h(n);
		}

		@Override
		public String toString() {
			return "A* evaluation";
		}
		
		public int h(Node n) {
			int score = 0;
			int bestScore = 0;
			for (int i = 0; i < n.boxes.length; i++) {
				for (int j = 0; j < n.boxes[i].length; j++) {
					if (n.boxes[i][j] == 'A') {//Theres a box at this point
						Point p = new Point(i,j);
						if (goals.contains(p)) {
							score = score - 5;//We have a box on top of a goal
						} else {
							int tempScore = calcScore(i, j, n);
							if (tempScore < bestScore) {
								bestScore = tempScore;
							}
						}
					}
				}
			}
			
			
			return score - bestScore;		}
	}

	public static class WeightedAStar extends Heuristic {
		private int W;

		public WeightedAStar(Node initialState, int W) {
			super(initialState);
			this.W = W;
		}
		private int calcScore(int i, int j, Node n) {//Calculates distance between a box and a goal
			int bestScore = 0xffffff;//Max int
			for (int s = 0; s < goals.size(); s++) {
				Point p = goals.get(s);
				int score = Math.abs(i-p.x) + Math.abs(j-p.y);
				if (score < bestScore) {
					bestScore = score;
				}
			}
			bestScore += Math.abs(i-n.agentRow) + Math.abs(j-n.agentCol);
			return bestScore;
		}

		@Override
		public int f(Node n) {
			return n.g() + this.W * this.h(n);
		}

		@Override
		public String toString() {
			return String.format("WA*(%d) evaluation", this.W);
		}
		public int h(Node n) {
			int score = 0;
			int bestScore = 0;
			for (int i = 0; i < n.boxes.length; i++) {
				for (int j = 0; j < n.boxes[i].length; j++) {
					if (n.boxes[i][j] == 'A') {//Theres a box at this point
						Point p = new Point(i,j);
						if (goals.contains(p)) {
							score = score - 5;//We have a box on top of a goal
						} else {
							int tempScore = calcScore(i, j, n);
							if (tempScore < bestScore) {
								bestScore = tempScore;
							}
						}
					}
				}
			}
			
			
			return score - bestScore;			
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
		
		private int calcScore(int i, int j, Node n) {//Calculates distance between a box and a goal
			int bestScore = 0xffffff;//Max int
			for (int s = 0; s < goals.size(); s++) {
				Point p = goals.get(s);
				int score = Math.abs(i-p.x) + Math.abs(j-p.y);
				if (score < bestScore) {
					bestScore = score;
				}
			}
			bestScore += Math.abs(i-n.agentRow) + Math.abs(j-n.agentCol);
			return bestScore;
		}

		@Override
		public String toString() {
			return "Greedy evaluation";
		}
		public int h(Node n) {//Get score from distance of nearest box to a goal
			int score = 0;
			int bestScore = 0;
			for (int i = 0; i < n.boxes.length; i++) {
				for (int j = 0; j < n.boxes[i].length; j++) {
					if (n.boxes[i][j] == 'A') {//Theres a box at this point
						Point p = new Point(i,j);
						if (goals.contains(p)) {
							score = score - 5;//We have a box on top of a goal
						} else {
							int tempScore = calcScore(i, j, n);
							if (tempScore < bestScore) {
								bestScore = tempScore;
							}
						}
					}
				}
			}
			
			
			return score - bestScore;
			
		}

	}
}
