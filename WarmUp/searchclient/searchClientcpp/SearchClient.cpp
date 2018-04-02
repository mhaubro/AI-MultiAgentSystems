#include "SearchClient.h"
#include <string>
#include <list>
#include <unordered_map>
#include <regex>
#include <iostream>
#include <iterator>
#include "Strategy.h"

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

		rows.push_back(line);

		getline(std::cin, line);
	} while (!std::cin.eof() && !line.compare(""));

	std::cerr << "Agents: " << agents <<
		"\nBoxes: " << boxes <<
		"\nDim: [" << cols << "," << rows.size() << "]\n";

	Node::MAX_ROW = rows.size();
	Node::MAX_COL = cols;

	Node::walls = std::vector<bool>(false, rows.size()*cols);
	Node::goals = std::vector<char>('-', rows.size()*cols);

	initialState = new Node(cols, rows.size());
	initialState->boxes = vector<char>('-', rows.size()*cols);
	for (int y = 0; y < rows.size(); y++){
		string row = rows.front();
		rows.pop_front();
		for (int x = 0; x < row.length(); x++){
			char chr = row[x];


			if (chr == '+'){
				Node::walls[x + y*cols] = true;
			} else if (isAgent(chr)){
				initialState->agentRow = y;
				initialState->agentCol = x;
			} else if (isBox(chr)){
				initialState->boxes[x+y*cols] = chr;
			} else if (isGoal(chr)){
				Node::goals[x+y*cols] = chr;
			} else {
				std::cerr << "Error, read invalid level character: [" << chr << "]" ;
			}
		}
	}
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
	std::cerr << "\nSummary for " << strategy.toString();
	std::cerr << "Found solution of length " << solution.size();
	std::cerr << strategy.searchStatus();

	for (Node * n : solution) {
			std::string act = n->action->to_string();
			std::cerr << act;
			std::string response;
			std::getline(std::cin, response);
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
				std::cerr << leafNode->toString();
				return leafNode->extractPlan();
			}

			strategy->addToExplored(leafNode);
			for (Node * n : leafNode->getExpandedNodes()) {
				if (!strategy->isExplored(n) && !strategy->inFrontier(n)) {
					strategy->addToFrontier(n);
				}
			}
			iterations++;
		}
	}
