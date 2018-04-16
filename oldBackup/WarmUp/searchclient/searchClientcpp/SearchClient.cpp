#include "SearchClient.h"
#include <string>
#include <list>
#include <unordered_map>
#include <regex>
#include <iostream>
#include <iterator>
#include "Strategy.h"
#include "Command.h"
#include "Memory.h"

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

SearchClient::SearchClient()
{
	/*
	 * This is a constructor that reads input from std::cin, from the server about the map
	 * This should not be necessary to edit, and if need be, most likely only in main method
	 * Where input parameters are read and might be added.
	 */
	std::unordered_map<char, std::string> colors;
	std::string line;

	std::regex color_regex("^[a-z]+:\\s*[0-9A-Z](\\s*,\\s*[0-9A-Z])*\\s*$");
	std::smatch match;

	/*
	This following segment reads input from the server, and should probably be in its own function
	Due to good style
	*/

	while (getline (std::cin, line) && !line.compare("") && std::regex_match(line, match, color_regex)) {

		printf("%s\n", line.c_str());
		stringstream ss(line);
		string color;
		getline(ss, color, ':');

		string c;
		while (getline(ss, c, ',')) {
			c.erase(remove(c.begin(), c.end(), ' '), c.end());
			colors.insert(pair<char,string>(c.c_str()[0], color));
		}

	}

	int agents = 0;
	int boxes = 0;
	for (auto it = colors.begin(); it != colors.end(); ++it) {
		if (isAgent(it->first)) {
			agents++;
		}

		if (isBox(it->first)) {
			boxes++;
		}
	}

	list<string> rows;

	int cols = 0;
	do
	{

		if (line.length() > cols) {
			cols = line.length();
		}

		rows.push_back(line);
		getline(std::cin, line);
	} while (!std::cin.eof() && (line.length() != 0));


	/*
	Read of input ends there
	*/

	/*
	Input is stored in a startnode. State is stored.
	*/

	std::cerr << "Agents: " << agents <<
		"\nBoxes: " << boxes <<
		"\nDim: [" << cols << "," << rows.size() << "]\n";

	Node::MAX_ROW = rows.size();
	Node::MAX_COL = cols;

	int size = rows.size();

	Node::walls.resize(rows.size()*cols, false);
	Node::goals.resize(rows.size()*cols, '\0');

	initialState = new Node();
	initialState->boxes = vector<char>(rows.size()*cols, '\0');

	for (int y = 0; y < size; y++){

		string row = rows.front();
		rows.pop_front();
		for (int x = 0; x < row.length(); x++){

			char chr = row[x];

			//This part is a bit iffy, as there's something odd with x and y, but IT WORKS.
			if (chr == '+'){
				Node::walls[y + x*size] = true;

			} else if (isAgent(chr)){
				initialState->agentRow = x;
				initialState->agentCol = y;
			} else if (isBox(chr)){
				initialState->boxes[y+x*size] = chr;
			} else if (isGoal(chr)){
				Node::goals[y+x*size] = chr;
			} else if (chr == ' '){
				//Do nothing, as nothing is printed
			}
		}
	}
	/*
	Everything is stored, so we print the state as a sanity check for the user
	*/
	std::cerr << initialState->toString();
}

//As we never expect to have more than 1, this is not implemented.
SearchClient::~SearchClient()
{
}


int main(int argc, char * argv[]){
	/*
	argv[1] contains memory limit in MB. This is the FIRST input, as argv[0] is program name
	argv[2] contains how often the print should happen. This is optional, default is 1000
	*/
	Memory::limit = atoi(argv[1]);
	std::cerr << Memory::limit << "mem\n";

	int printfreq = 1000;
	if (argc > 2){
		printfreq = atoi(argv[2]);
	}
	char buffer[200];

	SearchClient client = SearchClient();
	std::cerr << "Search started\n";

	StrategyBFS strategy = StrategyBFS();

	std::list<Node *> solution;
	solution = client.search(&strategy, printfreq);
	std::cerr << "\nSummary for " << strategy.toString() << ".\n";
	std::cerr << "Found solution of length " << solution.size() << ".\n";
	//std::cerr << strategy.searchStatus();

	for (Node * n : solution) {
		//std::cerr << "Printing solution1";
		if (n == NULL){
			std::cerr << "Something is null";
		}
		std::string act = n->action->to_string();
		//std::cerr << n->action->to_string();
		//std::cerr << "Printing solution2";
		std::cerr << act;
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


	std::list<Node *> SearchClient::search(Strategy * strategy, int printfrequency) {
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
			std::vector<Node *> nodes = leafNode->getExpandedNodes();
			for (Node * n : nodes) {
				if (!strategy->isExplored(n) && !strategy->inFrontier(n)) {
					strategy->addToFrontier(n);
				}
			}
			iterations++;
		}
	}
