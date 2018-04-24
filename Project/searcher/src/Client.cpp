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

Client::Client()
{
   initialState = Initializer::setupEnvironment();
	/*
	 * This is a constructor that reads input from std::cin, from the server about the map
	 * This should not be necessary to edit, and if need be, most likely only in main method
	 * Where input parameters are read and might be added.
	 */

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

	std::vector<std::string> solution;

	solution = MasterSearcher::getPlan(client.initialState);

	std::cerr << "\nSummary for " << strategy.toString() << ".\n";
	std::cerr << "Found solution of length " << solution.size() << ".\n";

	for (const auto & cmd : solution) {
		std::cout << cmd;
		std::cout << "\n";
		std::string response;
		//std::cerr << "Printing solution3";
		std::getline(std::cin, response);
		//std::cerr << "Printing solution4";
		if (response.find(std::string("false")) != std::string::npos) {
			sprintf(buffer, "Server responsed with %s to the inapplicable action: %s\n", response, cmd);
			std::cerr << std::string(buffer);
			//Maybe we should actually be able to print the state. However, sending
			//A plan assumes that the thing actually works, and we will never
			//Replan out here.
			sprintf(buffer, "%s was attempted in a state\n", cmd);
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
