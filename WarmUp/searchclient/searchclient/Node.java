package searchclient;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedList;
import java.util.Random;

import searchclient.Command.Type;

public class Node {
	private static final Random RND = new Random(1);

	public static int MAX_ROW = 70;
	public static int MAX_COL = 70;

	// Arrays are indexed from the top-left of the level, with first index being row and second being column.
	// Row 0: (0,0) (0,1) (0,2) (0,3) ...
	// Row 1: (1,0) (1,1) (1,2) (1,3) ...
	// Row 2: (2,0) (2,1) (2,2) (2,3) ...
	// ...
	// (Start in the top left corner, first go down, then go right)
	// E.g. this.walls[2] is an array of booleans having size MAX_COL.
	// this.walls[row][col] is true if there's a wall at (row, col)
	//

	public static boolean[][] walls = new boolean[MAX_ROW][MAX_COL];
	public static Goal[] goals;
	public Agent[] agents;
	// public Box[][] boxes = new Box[MAX_ROW][MAX_COL];
	public Box[] boxes;

	public Node parent;
	public Command action;

	private int g;
	
	private int _hash = 0;

	public Node(Node parent) {
		this.parent = parent;
		if (parent == null) {
			this.g = 0;
		} else {
			this.g = parent.g() + 1;
		}
	}

	public int g() {
		return this.g;
	}

	public boolean isInitialState() {
		return this.parent == null;
	}

	public boolean isGoalState()
	{
		for(Goal goal : goals)
		{
			if(!goal.goalState)
				return false;
		}
		return true;
	}

	public ArrayList<Node> getExpandedNodes()
	{
		ArrayList<Node> expandedNodes = new ArrayList<Node>(Command.EVERY.length);
		for(Agent a : agents)
		{
			int agentRow = a.location.getLeft();
			int agentCol = a.location.getRight();
			for (Command c : Command.EVERY)
			{
				// Determine applicability of action
				int newAgentRow = agentRow + Command.dirToRowChange(c.dir1);
				int newAgentCol = agentCol + Command.dirToColChange(c.dir1);

				if (c.actionType == Type.Move)
				{
					// Check if there's a wall or box on the cell to which the agent is moving
					if(this.cellIsFree(newAgentRow, newAgentCol))
					{
						Node n = this.ChildNode();
						n.action = c;
						n.getAgent(agentRow, agentCol).setLocation(new Pair<>(newAgentRow, newAgentCol));
						expandedNodes.add(n);
					}
				}
				else if (c.actionType == Type.Push)
				{
					// Make sure that there's actually a box to move
					if (this.boxAt(newAgentRow, newAgentCol))
					{
						int newBoxRow = newAgentRow + Command.dirToRowChange(c.dir2);
						int newBoxCol = newAgentCol + Command.dirToColChange(c.dir2);
						// .. and that new cell of box is free
						if (this.cellIsFree(newBoxRow, newBoxCol))
						{
                            Node n = this.ChildNode();
                            n.action = c;
							n.getAgent(agentRow, agentCol).setLocation(new Pair<>(newAgentRow, newAgentCol));
							n.getBox(newAgentRow, newAgentCol).setLocation(new Pair<>(newBoxRow, newBoxCol));
							// n.boxes[newBoxRow][newBoxCol] = this.boxes[newAgentRow][newAgentCol];
							// n.boxes[newAgentRow][newAgentCol] = 0;
							expandedNodes.add(n);
						}
					}
				}
				else if (c.actionType == Type.Pull)
				{
					// Cell is free where agent is going
					if (this.cellIsFree(newAgentRow, newAgentCol))
					{
						int boxRow = agentRow + Command.dirToRowChange(c.dir2);
						int boxCol = agentCol + Command.dirToColChange(c.dir2);
						// .. and there's a box in "dir2" of the agent
						if (this.boxAt(boxRow, boxCol))
						{
                            Node n = this.ChildNode();
                            n.action = c;
							n.getAgent(agentRow, agentCol).setLocation(new Pair<>(newAgentRow, newAgentCol));
							n.getBox(boxRow, boxCol).setLocation(new Pair<>(agentRow, agentCol));
							// n.boxes[this.agentRow][this.agentCol] = this.boxes[boxRow][boxCol];
							// n.boxes[boxRow][boxCol] = 0;
							expandedNodes.add(n);
						}
					}
				}
			}
		}
		Collections.shuffle(expandedNodes, RND);
		return expandedNodes;
	}

	private Box getBox(int row, int col)
	{
		for(Box box : this.boxes)
		{
			if(box.location.getLeft() == row && box.location.getRight() == col)
				return box;
		}
		throw new NullPointerException("No box at row: " + String.valueOf(row) + " and col: " + String.valueOf(col));
	}

	private Goal getGoal(int row, int col)
	{
		for(Goal goal : goals)
		{
			if(goal.location.getLeft() == row && goal.location.getRight() == col)
				return goal;
		}
		throw new NullPointerException("No goal at row: " + String.valueOf(row) + " and col: " + String.valueOf(col));
	}

	private Agent getAgent(int row, int col)
	{
		for(Agent a : this.agents)
		{
			if(a.location.getLeft() == row && a.location.getRight() == col)
				return a;
		}
		throw new NullPointerException("No agent at row: " + String.valueOf(row) + " and col: " + String.valueOf(col));
	}

	private boolean cellIsFree(int row, int col)
	{
		return !walls[row][col] && !boxAt(row, col) && !agentAt(row, col);
	}

	private boolean boxAt(int row, int col)
	{
		for(Box box : this.boxes)
		{
			if(box.location.getLeft() == row && box.location.getRight() == col)
				return true;
		}
		return false;
	}

	private boolean goalAt(int row, int col)
	{
		for(Goal goal : goals)
		{
			if(goal.location.getLeft() == row && goal.location.getRight() == col)
				return true;
		}
		return false;
	}

	private boolean agentAt(int row, int col)
	{
		for(Agent a : this.agents)
		{
			if(a.location.getLeft() == row && a.location.getRight() == col)
				return true;
		}
		return false;
	}

	private Node ChildNode() {
		Node copy = new Node(this);
		copy.boxes = DeepCloneBoxes(this.boxes);
		copy.agents = DeepCloneAgents(this.agents);
		return copy;
	}

	private Agent[] DeepCloneAgents(Agent[] agents){
		Agent[] clone = new Agent[agents.length];
		for(int i = 0; i < agents.length; i++){
			clone[i] = new Agent(agents[i]);
		}
		return clone;
	}

	private Box[] DeepCloneBoxes(Box[] boxes){
		Box[] clone = new Box[boxes.length];
		for(int i = 0; i < boxes.length; i++){
			clone[i] = new Box(boxes[i]);
		}
		return clone;
	}

	public LinkedList<Node> extractPlan() {
		LinkedList<Node> plan = new LinkedList<Node>();
		Node n = this;
		while (!n.isInitialState()) {
			plan.addFirst(n);
			n = n.parent;
		}
		return plan;
	}

	@Override
	public int hashCode() {
		if (this._hash == 0) {
			final int prime = 31;
			int result = 1;
			result = prime * result + Arrays.deepHashCode(this.agents);
			result = prime * result + Arrays.deepHashCode(this.boxes);
			//result = prime * result + Arrays.deepHashCode(Node.goals);
			//result = prime * result + Arrays.deepHashCode(Node.walls);
			this._hash = result;
		}
		return this._hash;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (this.getClass() != obj.getClass())
			return false;
		Node other = (Node) obj;
		if(!Arrays.deepEquals(this.agents,other.agents))
			return false;
		//if (!Arrays.deepEquals(this.boxes, other.boxes))
		if(!Arrays.deepEquals(this.boxes, other.boxes))
			return false;
		// if (!Arrays.deepEquals(this.goals, other.goals))
			// return false;
		//if (!Arrays.deepEquals(this.walls, other.walls))
		//	return false;
		return true;
	}

	@Override
	public String toString() {
		StringBuilder s = new StringBuilder();
		for (int row = 0; row < MAX_ROW; row++) {
			if (!this.walls[row][0]) {
				break;
			}
			for (int col = 0; col < MAX_COL; col++)
			{
				if (boxAt(row, col)) {
					s.append(getBox(row, col).chr);
				} else if (goalAt(row, col)) {
					s.append(getGoal(row, col).chr);
				} else if (this.walls[row][col]) {
					s.append("+");
				} else if (agentAt(row, col)) {
					s.append(getAgent(row, col).num);
				} else {
					s.append(" ");
				}
			}
			s.append("\n");
		}
		return s.toString();
	}

}