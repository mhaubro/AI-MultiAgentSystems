#include "Node.h"
#include "centralHeader.h"
#include <string>
#include <vector>
#include <list>
#define RANDOM_SEED 1

	//Initialize static variables:
	int Node::MAX_ROW = 70;
	int Node::MAX_COL = 70;
	std::vector<bool> Node::walls = std::vector<bool>(false, MAX_ROW*MAX_COL);
	std::vector<char> Node::goals = std::vector<char>('-', MAX_ROW*MAX_COL);

	//This should only be used for the first node. Inits walls & goals.

	Node::Node(int MAX_COL, int MAX_ROW) {
		this->parent = NULL;
		this->MAX_ROW = MAX_ROW;
		this->MAX_COL = MAX_COL;
		this->walls = std::vector<bool>(false, MAX_ROW*MAX_COL);
		this->goals = std::vector<char>('-', MAX_ROW*MAX_COL);
		this->gval = 0;
		this->boxes = std::vector<char>(0, MAX_COL*MAX_ROW);
	}

	Node::Node(Node * parent) {
		this->parent = parent;
		this->gval = parent->g() + 1;

		//Inits boxes
		this->boxes = std::vector<char>(0, MAX_COL*MAX_ROW);
	}

	int Node::g() {
		return this->gval;
	}

	bool Node::isInitialState() {
		return this->parent == NULL;
	}

	bool Node::isGoalState() {
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

	std::vector Node::getExpandedNodes() {
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
						n.boxes[newBoxRow+newBoxCol*MAX_ROW] = this.boxes[newAgentRow+newAgentCol*MAX_ROW];
						n.boxes[newAgentRow+newAgentCol*MAX_ROW] = 0;
						expandedNodes.add(n);
					}
				}
			} else if (c.actionType == Type.Pull) {
				// Cell is free where agent is going
				if (this.cellIsFree(newAgentRow, newAgentCol)) {
					int boxRow = this->agentRow + Command.dirToRowChange(c->dir2);
					int boxCol = this->agentCol + Command.dirToColChange(c->dir2);
					// .. and there's a box in "dir2" of the agent
					if (this.boxAt(boxRow, boxCol)) {
						Node n = this->ChildNode();
						n.action = c;
						n.agentRow = newAgentRow;
						n.agentCol = newAgentCol;
						n.boxes[this->agentRow+this->agentCol*MAX_ROW] = this.boxes[boxRow+boxCol*MAX_ROW];
						n.boxes[boxRow+boxCol*MAX_ROW] = 0;
						expandedNodes.add(n);
					}
				}
			}
		}
		Collections.shuffle(expandedNodes, RND);
		return expandedNodes;
	}

	bool Node::cellIsFree(int row, int col) {
		return !walls[row+col*MAX_ROW] && boxes[row+col*MAX_ROW] == 0;
	}

	bool Node::boxAt(int row, int col) {
		return boxes[row+col*MAX_ROW] > 0;
	}

	Node Node::ChildNode() {
		Node copy = new Node(this);
		//This works because std::vector. Copies full 1D-array. Thank god for 1D :)
		copy.boxes = this->boxes;
		return copy;
	}

	std::list<Node> Node::extractPlan() {
		std::list<Node> plan = std::list<Node>();
		Node n = this;
		while (!n.isInitialState()) {
			plan.addFirst(n);
			n = n->parent;
		}
		return plan;
	}

	int Node::hashCode() {
		if (this->_hash == 0) {
			final int prime = 31;
			int result = 1;
			result = prime * result + this->agentCol;
			result = prime * result + this->agentRow;
			result = prime * result + Arrays.deepHashCode(this->boxes);
			result = prime * result + Arrays.deepHashCode(this->goals);
			result = prime * result + Arrays.deepHashCode(this->walls);
			this->_hash = result;
		}
		return this->_hash;
	}

	bool Node::equals(Node * obj) {
		if (this == obj)
			return true;
		if (obj == NULL)
			return false;
//		if (this.getClass() != obj.getClass())//This is assumed
//			return false;
		if (this->agentRow != obj->agentRow || this->agentCol != obj->agentCol)
			return false;
			//This again works in std::vector
		if (!(this->boxes == obj->boxes))
			return false;
		return true;
	}

	std::string Node::toString() {
		std::string s;
		for (int row = 0; row < MAX_ROW; row++) {
			if (!this->walls[row]) {
				break;
			}
			for (int col = 0; col < MAX_COL; col++) {
				if (this->boxes[row+col*MAX_ROW] > 0) {
					s += (this->boxes[row+col*MAX_ROW]);
				} else if (this->goals[row+col*MAX_ROW] > 0) {
					s += (this->goals[row+col*MAX_ROW]);
				} else if (this->walls[row+col*MAX_ROW]) {
					s += ("+");
				} else if (row == this->agentRow && col == this->agentCol) {
					s += ("0");
				} else {
					s += (" ");
				}
			}
			s.append("\n");
		}
		return s;
	}
