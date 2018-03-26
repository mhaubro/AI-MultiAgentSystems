#include <string>
#include <list>
#include <unordered_map>
#include <regex>
#include <iostream>
#include <iterator>

using std::stringstream;
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

	getline(serverMessages, line);
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
