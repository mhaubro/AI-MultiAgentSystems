#include "Client.h"
#include <string>
#include <list>
#include <unordered_map>
#include <regex>
#include <iostream>
#include <iterator>
#include "Strategy.h"
#include "Command.h"
#include "Memory.h"
#include "MasterSearcher.h"
#include "Initializer.h"

using std::stringstream;
using std::istream;
using std::vector;
using std::string;
using std::getline;
using std::pair;
using std::list;

bool isAgent(char c) {
	return ('0' <= c && c <= '9');
}

bool isBox(char c) {
	return ('A' <= c && c <= 'Z');
}

bool isGoal(char c) {
	return ('a' <= c && c <= 'z');
}

Client::Client()
{
	Initializer::setupEnvironment();
	/*
	 * This is a constructor that reads input from std::cin, from the server about the map
	 * This should not be necessary to edit, and if need be, most likely only in main method
	 * Where input parameters are read and might be added.
	 */
	 std::vector<Box> boxes = std::vector<Box>();
 	std::vector<Agent> agents = std::vector<Agent>();
 	std::vector<Goal> goals = std::vector<Goal>();



	/*
	Read of input ends there
	*/

	/*
	Input is stored in a startnode. State is stored.
	*/

	std::cerr << "Agents: " << agentnum <<
		"\nBoxes: " << boxnum <<
		"\nDim: [" << cols << "," << rows.size() << "]\n";

	int ysize = rows.size();
	int xsize = cols;
	Node::maxX = xsize;
	Node::maxY = ysize;

	Node::walls.resize(xsize*ysize, false);

	initialState = new Node();

	for (int y = 0; y < ysize; y++){
		string row = rows.front();
		rows.pop_front();
		for (int x = 0; x < row.length(); x++){

			char chr = row[x];

			if (chr == '+'){
				Node::walls[x + y*xsize] = true;

			} else if (isAgent(chr)){
				agents.emplace_back((int)(chr - '0'), std::pair<int, int>(x, y));
			} else if (isBox(chr)){
				boxes.emplace_back(chr, std::pair<int, int>(x, y));
			} else if (isGoal(chr)){
				goals.emplace_back(chr, std::pair<int, int>(x, y));
			} else if (chr == ' '){
				//Do nothing, as nothing is printed
			}
		}
	}
	Node::goals = goals;
	this->initialState->boxes = boxes;
	this->initialState->agents = agents;
	/*
	Everything is stored, so we print the state as a sanity check for the user
	*/
	std::cerr << initialState->toString();
}

//As we never expect to have more than 1, this is not implemented.
Client::~Client()
{
}


int main(int argc, char * argv[]){
	/*
	argv[1] contains memory limit in MB. This is the FIRST input, as argv[0] is program name
	argv[2] contains how often the print should happen. This is optional, default is 1000
	*/
	Memory::limit = atoi(argv[1]);

	int printfreq = 1000;
	if (argc > 2){
		printfreq = atoi(argv[2]);
	}
	char buffer[200];

	Client client = Client();
	std::cerr << "Search started\n";

	StrategyBFS strategy = StrategyBFS();

	std::list<Node *> solution;
	solution = client.search(&strategy, printfreq);
	std::cerr << "\nSummary for " << strategy.toString() << ".\n";
	std::cerr << "Found solution of length " << solution.size() << ".\n";

	for (const auto & n : solution) {
		//std::cerr << "Printing solution1";
		std::string act = n->action->to_string();
		std::cout << act;
		std::cout << "\n";
		std::string response;
		//std::cerr << "Printing solution3";
		std::getline(std::cin, response);
		//std::cerr << "Printing solution4";
		if (response.find(std::string("false")) != std::string::npos) {
			sprintf(buffer, "Server responsed with %s to the inapplicable action: %s\n", response, act);
			std::cerr << std::string(buffer);
			sprintf(buffer, "%s was attempted in \n%s\n", act, n->toString());
			std::cerr << std::string(buffer);
			break;
		}
		}
		return 0;
	}

	/*
	 * Here an actual search is being done. This is so critical, it might deserve it's own class.
	 */


	std::list<Node *> Client::search(Strategy * strategy, int printfrequency) {
		char buffer[100];
		sprintf(buffer, "Search starting with strategy %s.\n", strategy->toString().c_str());
		std::string s = std::string(buffer);
		std::cerr << s;
		strategy->addToFrontier(this->initialState);
		int iterations = 0;

		while (true) {
			if (iterations % printfrequency == 0) {
				strategy->searchStatus(iterations);
			}

			if (strategy->frontierIsEmpty()) {
				//Empty list
				return std::list<Node *>();
			}

			Node * leafNode = strategy->getAndRemoveLeaf();

			if (leafNode->isGoalState()) {
				//A goal is found, final state is printed
				std::cerr << "Goal!\n";
				strategy->searchStatus(iterations);
				std::cerr << leafNode->toString();
				return leafNode->extractPlan();
			}

			strategy->addToExplored(leafNode);
			//Gets all new nodes
			std::vector<Node> nodes = leafNode->getExpandedNodes();
			for (auto & n : nodes) {
				if (!strategy->isExplored(&n) && !strategy->inFrontier(&n)) {
					strategy->addToFrontier(Node::getopCopy(&n));
				}
			}
			iterations++;
		}
	}
