
#include "Command.h"
#include <string>
#include <vector>
#include <list>
#include <cstring>
#include <iostream>

	/*
	Initializing EVERY through a lambda expression. Requires C++-11
	This works, gives every single possible command except NoOp.
	TODO: Add NOOP. Maybe add other commands as well.
	*/
	std::vector<Command *> Command::EVERY = []() -> std::vector<Command *> {
		std::list<Command *> cmds = std::list<Command *>();
		for (int d1 = 0; d1 < Command::LASTD; d1++) {
			for (int d2 = 0; d2 < Command::LASTD; d2++) {
				if (!Command::isOpposite(d1, d2)) {
					cmds.push_back(new Command(1, d1, d2));
				}
			}
		}
		for (int d1 = 0; d1 < Command::LASTD; d1++) {
			for (int d2 = 0; d2 < Command::LASTD; d2++) {
				if (d1 != d2) {
					cmds.push_back(new Command(2, d1, d2));
				}
			}
		}
		for (int d = 0; d < Command::LASTD; d++) {
			cmds.push_back(new Command(d));
		}

		return 		std::vector<Command *> { std::make_move_iterator(std::begin(cmds)),
                  std::make_move_iterator(std::end(cmds)) };;
	}();//Lambda ends here

	bool Command::isOpposite(int d1, int d2) {
		return d1 + d2 == 3;
	}

	int Command::dirToRowChange(int d) {
		// South is down one row (1), north is up one row (-1).
		switch (d) {
		case S:
			return 1;
		case N:
			return -1;
		default:
			return 0;
		}
	}

	int Command::dirToColChange(int d) {
		// East is right one column (1), west is left one column (-1).
		switch (d) {
		case E:
			return 1;
		case W:
			return -1;
		default:
			return 0;
		}
	}

	Command::Command(int d) {
		this->actionType = Command::Move;
		this->dir1 = d;
		this->dir2 = -1;//Indicates illegal direction
	}

	Command::Command(int t, int d1, int d2) {
		this->actionType = t;
		this->dir1 = d1;
		this->dir2 = d2;
	}

	//Destructor
	Command::~Command(){

	}

	std::string Command::to_string() {
		static const char DirNames[4][1+1] = {"N", "W", "E", "S"};
		static const char TypeNames[3][4+1] = {"Move", "Push", "Pull"};
		std::string s = "";
		char newC[12];

		if (this->actionType == Command::Move){
			//Command will be 5 chars long
			s.resize(9);
			sprintf(newC, "[%s(%s)]", TypeNames[this->actionType], DirNames[this->dir1]);
			std::memcpy(&s[0], newC, 9);
			return s;
		} else {
			s.resize(11);
			sprintf(newC, "[%s(%s,%s)]", TypeNames[this->actionType], DirNames[this->dir1], DirNames[this->dir2]);
			std::memcpy(&s[0], newC, 11);
			return s;
		}
	}
