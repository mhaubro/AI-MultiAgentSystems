#include "SearchClient.h"
#include <string>
#include <list>
#include <unordered_map>
#include <regex>
#include <iostream>
#include <iterator>

using std::stringstream;
using std::istream;
using std::vector;
using std::string;
using std::getline;
using std::pair;
using std::list;



bool isAgent(char c) {
	return ('0' <= chr && chr <= '9');
}

bool isBox(char c) {
	return ('A' <= chr && chr <= 'Z');
}

bool isGoal(char c) {
	return ('a' <= chr && chr <= 'z');
}

SearchClient::SearchClient(std::stringstream serverMessages)
{
	std::unordered_map<char, std::string> colors;
	std::string line;

	std::regex color_regex("^[a-z]+:\\s*[0-9A-Z](\\s*,\\s*[0-9A-Z])*\\s*$");
	std::smatch match;

	getline(line);
	while (!serverMessages.eof() && std::regex_match(line, match, color_regex)) {
		printf("%s\n", line.c_str());

		stringstream ss(line);
		string color;
		getline(ss, color, ':');

		string c;
		while (getline(ss, c, ',')) {
			c.erase(remove(c.begin(), c.end(), ' '), c.end());
			colors.insert(pair<char,string>(c.c_str()[0], color));
		}
		getline(serverMessages, line);
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
	bool eof = false;
	do
	{
		//moreLines = !serverMessages.eof();
		if (line.length() > cols) {
			cols = line.length();
		}

		rows.push_back(line);

		eof = serverMessages.eof();
		getline(serverMessages, line);
	} while (!eof);


	std::cout << "Agents: " << agents <<
		"\nBoxes: " << boxes <<
		"\nDim: [" << cols << "," << rows.size() << "]\n";

	Node::MAX_ROWS = rows.size()
	Node::MAX_COLS = cols;

	std::vector<bool> Node::walls = std::vector<bool>(false, rows.size()*cols);
	std::vector<char> Node::goals = std::vector<char>('-', rows.size()*cols);


	initialState = new Node(NULL);
	initialState->boxes = vector<char>('-', rows.size()*cols);
	for (int y = 0; y < rows.size(); y++){
		string row = rows.pull_front();
		for (int x = 0; x < rows.length(); x++){
			char chr = string[x];

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
				std::err("Error, read invalid level character: [" + chr + "]");
			}
		}
	}
}


SearchClient::~SearchClient()
{
}


int main(int argc, char * argv[]){
	//stringstream
	int x;
	for (int i = 0; i < argc; i++){
		std::cout << argv[i] << "\n";
	}

	std::cerr << "SearchClient started";
	streambuffer s = new streambuffer();
	std::istream sstream = std::istream();
	SearchClient client = new SearchClient(sstream);

	std::string strategy = std::string(argv[1])
	std::cout << strategy;

	Strategy strategy;
	if (argc > 0) {
			switch (strategy) {
					case std::string("-bfs"):
							strategy = new StrategyBFS();
							break;
/*					case "-dfs":
							strategy = new StrategyDFS();
							break;
					case "-astar":
							strategy = new StrategyBestFirst(new AStar(client.initialState));
							break;
					case "-wastar":
							// You're welcome to test WA* out with different values, but for the report you must at least indicate benchmarks for W = 5.
							strategy = new StrategyBestFirst(new WeightedAStar(client.initialState, 5));
							break;
					case "-greedy":
							strategy = new StrategyBestFirst(new Greedy(client.initialState));
							break;
*/					default:
							strategy = new StrategyBFS();
							std::cout << "Defaulting to BFS search. Use arguments -bfs, -dfs, -astar, -wastar, or -greedy to set the search strategy.";
			}
	} else {
			strategy = new StrategyBFS();
			std::cout << "Defaulting to BFS search. Use arguments -bfs, -dfs, -astar, -wastar, or -greedy to set the search strategy.";
	}

		std::list<Node *> solution;
//	try {
		solution = client.Search(strategy);
/*	} catch () {
		System.err.println("Maximum memory usage exceeded.");
		solution = null;
	}*/

/*	if (solution == null) {
		System.err.println(strategy.searchStatus());
		System.err.println("Unable to solve level.");
		System.exit(0);
	} else {*/
		std::cerr << "\nSummary for " << strategy.toString();
		std::cerr << "Found solution of length " << solution.size();
		std::cerr << strategy.searchStatus();

		for (Node &n : solution) {
			std::string act = n->action.toString();
			std::cerr << act;
			std::string response = serverMessages.readLine();
			if (response.find(std::string("false")) != std::string::npos) {
				std::cerr << sprintf("Server responsed with %s to the inapplicable action: %s\n", response, act);
				std::cerr << sprintf("%s was attempted in \n%s\n", act, n.toString());
				break;
			}
		}
		return 0;
	}
