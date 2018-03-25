#include "Node.h"
#include "centralHeader.h"
#include <string>
#include <vector>
#include <list>
#define RANDOM_SEED 1

namespace Node{
	//Initialize static variables:
	static std::vector<bool> walls = std::vector<bool>();
	static std::vector<char> goals; = new char[MAX_ROW][MAX_COL];

	//This should only be used for the first node. Inits walls & goals.
	Node(int MAX_COL, int MAX_ROW) {
		this->parent = NULL;
		this->MAX_ROW = MAX_ROW;
		this->MAX_COL = MAX_COL;
		this->walls = std::vector<bool>(false, MAX_ROW*MAX_COL);
		this->goals = std::vector<char>('-', MAX_ROW*MAX_COL);
		this->g = 0;
		boxes = std::vector(0, MAX_COL*MAX_ROW);
	}

	Node(Node * parent) {
		this->parent = parent;
		this->g = parent->g() + 1;
		}
		//Inits boxes
		boxes = std::vector(0, MAX_COL*MAX_ROW);
	}

	int g() {
		return this->g;
	}

	bool isInitialState() {
		return this->parent == NULL;
	}

	bool isGoalState() {
		for (int row = 0; row < MAX_ROW - 1; row++) {
			for (int col = 0; col < MAX_COL - 1; col++) {
				int idx = row + col*MAX_ROW;
				char g = goals[idx]];
				char b = Character.toLowerCase(boxes[idx]);
				if (g > 0 && b != g) {
					return false;
				}
			}
		}
		return true;
	}

	std::vector getExpandedNodes() {
		std::vector<Node> expandedNodes = std::vector<Node>(Command.EVERY.size());
		for (Command &c : Command.EVERY) {
			// Determine applicability of action
			int newAgentRow = this->agentRow + Command.dirToRowChange(c->dir1);
			int newAgentCol = this->agentCol + Command.dirToColChange(c->dir1);

			if (c->actionType == Command.Type.Move) {
				// Check if there's a wall or box on the cell to which the agent is moving
				if (this->cellIsFree(newAgentRow, newAgentCol)) {
					Node n = this.ChildNode();
					n->action = c;
					n->agentRow = newAgentRow;
					n->agentCol = newAgentCol;
					expandedNodes.add(n);
				}
			} else if (c->actionType == Command.Type.Push) {
				// Make sure that there's actually a box to move
				if (this.boxAt(newAgentRow, newAgentCol)) {
					int newBoxRow = newAgentRow + Command.dirToRowChange(c->dir2);
					int newBoxCol = newAgentCol + Command.dirToColChange(c->dir2);
					// .. and that new cell of box is free
					if (this->cellIsFree(newBoxRow, newBoxCol)) {
						Node n = this->ChildNode();
						n.action = c;
						n.agentRow = newAgentRow;
						n.agentCol = newAgentCol;
						n.boxes[newBoxRow][newBoxCol] = this.boxes[newAgentRow][newAgentCol];
						n.boxes[newAgentRow][newAgentCol] = 0;
						expandedNodes.add(n);
					}
				}
			} else if (c.actionType == Type.Pull) {
				// Cell is free where agent is going
				if (this.cellIsFree(newAgentRow, newAgentCol)) {
					int boxRow = this.agentRow + Command.dirToRowChange(c.dir2);
					int boxCol = this.agentCol + Command.dirToColChange(c.dir2);
					// .. and there's a box in "dir2" of the agent
					if (this.boxAt(boxRow, boxCol)) {
						Node n = this.ChildNode();
						n.action = c;
						n.agentRow = newAgentRow;
						n.agentCol = newAgentCol;
						n.boxes[this.agentRow][this.agentCol] = this.boxes[boxRow][boxCol];
						n.boxes[boxRow][boxCol] = 0;
						expandedNodes.add(n);
					}
				}
			}
		}
		Collections.shuffle(expandedNodes, RND);
		return expandedNodes;
	}

	bool cellIsFree(int row, int col) {
		return !walls[row][col] && boxes[row][col] == 0;
	}

	bool boxAt(int row, int col) {
		return boxes[row][col] > 0;
	}

	Node ChildNode() {
		Node copy = new Node(this);
		for(int row = 0; row < MAX_ROW; row++)
		    System.arraycopy(this.boxes[row], 0, copy.boxes[row], 0, MAX_COL);
		return copy;
	}

	std::list<Node> extractPlan() {
		LinkedList<Node> plan = new LinkedList<Node>();
		Node n = this;
		while (!n.isInitialState()) {
			plan.addFirst(n);
			n = n.parent;
		}
		return plan;
	}

	int hashCode() {
		if (this._hash == 0) {
			final int prime = 31;
			int result = 1;
			result = prime * result + this.agentCol;
			result = prime * result + this.agentRow;
			result = prime * result + Arrays.deepHashCode(this.boxes);
			result = prime * result + Arrays.deepHashCode(this.goals);
			result = prime * result + Arrays.deepHashCode(this.walls);
			this._hash = result;
		}
		return this._hash;
	}

	bool equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (this.getClass() != obj.getClass())
			return false;
		Node other = (Node) obj;
		if (this.agentRow != other.agentRow || this.agentCol != other.agentCol)
			return false;
		if (!Arrays.deepEquals(this.boxes, other.boxes))
			return false;
		if (!Arrays.deepEquals(this.goals, other.goals))
			return false;
		if (!Arrays.deepEquals(this.walls, other.walls))
			return false;
		return true;
	}

	std::string toString() {
		StringBuilder s = new StringBuilder();
		for (int row = 0; row < MAX_ROW; row++) {
			if (!this.walls[row][0]) {
				break;
			}
			for (int col = 0; col < MAX_COL; col++) {
				if (this.boxes[row][col] > 0) {
					s.append(this.boxes[row][col]);
				} else if (this.goals[row][col] > 0) {
					s.append(this.goals[row][col]);
				} else if (this.walls[row][col]) {
					s.append("+");
				} else if (row == this.agentRow && col == this.agentCol) {
					s.append("0");
				} else {
					s.append(" ");
				}
			}
			s.append("\n");
		}
		return s.toString();
	}

}
