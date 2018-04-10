#include "SearchClient.h"
#include <string>
#include <list>
#include <unordered_map>
#include <regex>
#include <iostream>
#include <iterator>
#include "Strategy.h"
#include "Command.h"

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
	std::unordered_map<char, std::string> colors;
	std::string line;

	std::regex color_regex("^[a-z]+:\\s*[0-9A-Z](\\s*,\\s*[0-9A-Z])*\\s*$");
	std::smatch match;


	while (getline (std::cin, line) && !line.compare("") && std::regex_match(line, match, color_regex)) {
		std::cerr << "Hi!";
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
		//moreLines = !serverMessages.eof();
		if (line.length() > cols) {
			cols = line.length();
		}

//		std::cerr << line;
		rows.push_back(line);
		getline(std::cin, line);
	} while (!std::cin.eof() && (line.length() != 0));

	std::cerr << "Agents: " << agents <<
		"\nBoxes: " << boxes <<
		"\nDim: [" << cols << "," << rows.size() << "]\n";

//The max number of a row index is the amount of columns.
	Node::MAX_ROW = rows.size();
	Node::MAX_COL = cols;

	int size = rows.size();

	Node::walls.resize(rows.size()*cols, false);
	Node::goals.resize(rows.size()*cols, '\0');


	initialState = new Node();
	initialState->boxes = vector<char>(rows.size()*cols, '\0');
	//std::cerr << "cols is equal to " << cols << "\n";
	for (int y = 0; y < size; y++){
	//	std::cerr << "Y is equal to " << y << "\n";
		string row = rows.front();
		rows.pop_front();
		for (int x = 0; x < row.length(); x++){
	//		std::cerr << "X is equal to " << x << "\n";
			char chr = row[x];


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
				//Do nothing
			} else {
	//			std::cerr << "Error, read invalid level character: [" << chr << "]" ;
			}
		}
	}
	std::cerr << initialState->toString();

}


SearchClient::~SearchClient()
{
}


int main(int argc, char * argv[]){
	//stringstream
	char buffer[200];
	int x;
	for (int i = 0; i < argc; i++){
		std::cerr << argv[i] << "\n";
	}

	SearchClient client = SearchClient();

	std::string strat = std::string(argv[0]);
	std::cerr << strat;
	std::cerr << "SearchClient started\n";

	StrategyBFS strategy = StrategyBFS();
	std::cerr << "Defaulting to BFS search. Use arguments -bfs, -dfs, -astar, -wastar, or -greedy to set the search strategy.\n";

	std::list<Node *> solution;
	solution = client.search(&strategy);
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

	std::list<Node *> SearchClient::search(Strategy * strategy) {
		char buffer[100];
		sprintf(buffer, "Search starting with strategy %s.\n", strategy->toString().c_str());
		std::string s = std::string(buffer);
		std::cerr << s;
		strategy->addToFrontier(this->initialState);
		int iterations = 0;

		while (true) {
			//std::cerr << "Iteration!\n";
			if (iterations == 1000) {
				std::cerr << strategy->searchStatus();
				iterations = 0;
			}

			if (strategy->frontierIsEmpty()) {
				//Empty list
				return std::list<Node *>();
			}

			Node * leafNode = strategy->getAndRemoveLeaf();

			if (leafNode->isGoalState()) {
				std::cerr << "Goal!\n";
				std::cerr << leafNode->toString();
				return leafNode->extractPlan();
			}

			strategy->addToExplored(leafNode);
			std::vector<Node *> nodes = leafNode->getExpandedNodes();
			//std::cerr << "Number of new elements: " << nodes.size() << "\n";
			for (Node * n : nodes) {
				if (!strategy->isExplored(n) && !strategy->inFrontier(n)) {
					strategy->addToFrontier(n);
				}
			}
			iterations++;
		}
	}
