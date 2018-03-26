#ifndef STRATEGY_H
#define STRATEGY_H
#include "centralHeader.h"
#include <string>
#include "memory.h"
#include "Node.h"

abstract class Strategy {
private:
	HashSet<Node> explored;
	long startTime;

public:

	Strategy();
	void addToExplored(Node n);
	public boolean isExplored(Node n);
	public int countExplored();
	public std::string searchStatus();
	public float timeSpent();
	public virtual Node getAndRemoveLeaf();
	public virtual void addToFrontier(Node n);
	public virtual bool inFrontier(Node n);
	public virtual int countFrontier();
	public virtual bool frontierIsEmpty();
	public virtual std::string toString();

}
	public static class StrategyBFS extends Strategy {
		ArrayDeque<Node> frontier;
		HashSet<Node> frontierSet;

		StrategyBFS() {
			frontier = new ArrayDeque<Node>();
			frontierSet = new HashSet<Node>();
		}

		Node getAndRemoveLeaf() {
			Node n = frontier.pollFirst();
			frontierSet.remove(n);
			return n;
		}

		void addToFrontier(Node n) {
			frontier.addLast(n);
			frontierSet.add(n);
		}

		int countFrontier() {
			return frontier.size();
		}

		bool frontierIsEmpty() {
			return frontier.isEmpty();
		}

		bool inFrontier(Node n) {
			return frontierSet.contains(n);
		}

    std::string toString() {
			return std::string("Breadth-first Search");
		}
	}

	public static class StrategyDFS extends Strategy {
		private ArrayDeque<Node> frontier;
		private HashSet<Node> frontierSet;

		public StrategyDFS() {
			super();
			frontier = new ArrayDeque<Node>();
			frontierSet = new HashSet<Node>();
		}

		@Override
		public Node getAndRemoveLeaf() {
			Node n = frontier.pollFirst();
			frontierSet.remove(n);
			return n;
		}

		@Override
		public void addToFrontier(Node n) {
		    frontier.addFirst(n);
		    frontierSet.add(n);
		}

        @Override
        public int countFrontier() {
            return frontier.size();
        }

        @Override
        public boolean frontierIsEmpty() {
            return frontier.isEmpty();
        }

        @Override
        public boolean inFrontier(Node n) {
            return frontierSet.contains(n);
        }

		@Override
		public String toString() {
			return "Depth-first Search";
		}
	}

	// Ex 3: Best-first Search uses a priority queue (Java contains no implementation of a Heap data structure)
	public static class StrategyBestFirst extends Strategy {
		private Heuristic heuristic;
		private PriorityQueue<Node> frontier;
		private HashSet<Node> frontierSet;

		public StrategyBestFirst(Heuristic h)
        {
            super();
            this.heuristic = h;
            frontier = new PriorityQueue<Node>(10, this.heuristic);
            frontierSet = new HashSet<Node>();
        }

		@Override
		public Node getAndRemoveLeaf() {
			Node n = frontier.poll();
			frontierSet.remove(n);
			return n;
		}

		@Override
		public void addToFrontier(Node n) {
			frontier.add(n);
		}

		@Override
		public int countFrontier() {
			return frontier.size();
		}

		@Override
		public boolean frontierIsEmpty() {
			return frontier.isEmpty();
		}

		@Override
		public boolean inFrontier(Node n) {
			return frontier.contains(n);
		}

		public std::String toString();
  }
}

#endif
