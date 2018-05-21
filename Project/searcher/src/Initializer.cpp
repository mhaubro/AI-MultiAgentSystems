#include "Initializer.h"
#include "Node.h"
#include <string>
#include <sstream>
#include <list>
#include <unordered_map>
#include <regex>
#include <iostream>
#include <iterator>
#include <algorithm>

using std::string;
using std::stringstream;

//Used for sorting agent array, cause I'm lazy /Martin
bool compAgents (Agent & i,Agent & j) { return (i.getChar() < j.getChar()); }

namespace Initializer {
bool isAgent(char c) {
	return ('0' <= c && c <= '9');
}

bool isBox(char c) {
	return ('A' <= c && c <= 'Z');
}

bool isGoal(char c) {
	return ('a' <= c && c <= 'z');
}

Node * storeInput(std::list<std::string> rows, int cols, std::unordered_map<char, Entity::COLOR> colors){
	using namespace std;

	std::vector<Box> boxes = std::vector<Box>();
	std::vector<Agent> agents = std::vector<Agent>();

	int ysize = rows.size();
	int xsize = cols;
	Node::maxX = xsize;
	Node::maxY = ysize;

	Node::walls.resize(xsize*ysize, false);

	Node * initialState = new Node();

	for (int y = 0; y < ysize; y++){
		string row = rows.front();
		rows.pop_front();
		for (int x = 0; x < row.length(); x++){
			char chr = row[x];
			if (chr == '+'){
				Node::walls[x + y*xsize] = true;
			} else if (isAgent(chr)){
 				agents.emplace_back(Agent(chr, Location(x, y), colors.find(chr)->second));
			} else if (isBox(chr)){
				boxes.emplace_back(chr, Location(x, y), colors.find(chr)->second);
			} else if (isGoal(chr)){
				Node::goals.emplace_back(chr, Location(x, y));
			} else if (chr == ' '){
				//Do nothing, as nothing is printed
			}
		}
	}

	std::sort (agents.begin(), agents.end(), compAgents);
	initialState->boxes = boxes;
	initialState->agents = agents;

	cerr << initialState->toString();

	return initialState;
}


Node * readSingleAgentLevel(const string level){

	std::unordered_map<char, Entity::COLOR> colors;

	int numAgents = 0, numBoxes = 0;

	for (int i = 0; i < level.length(); i++){
		char c = level[i];
		if (c <= '9' && c >= '0'){
			colors.insert(std::pair<char,Entity::COLOR>(c, Entity::BLUE));
			numAgents++;
		}else if (c <= 'Z' && c >= 'A'){
			colors.insert(std::pair<char,Entity::COLOR>(c, Entity::BLUE));
			numBoxes++ ;
		}
	}

	std::list<std::string> rows;
	string line;
	stringstream ss(level);
	getline(ss, line);


	int cols = 0;
	do
	{

		if (line.length() > cols) {
			cols = line.length();
		}

		rows.push_back(line);
		getline(ss, line);
	} while (!ss.eof() && (line.length() != 0));

	//std::cerr << "Agents: " << numAgents <<
	//"\nBoxes: " << numBoxes <<
	//"\nDim: [" << cols << "," << rows.size() << "]\n";

	return storeInput(rows, cols, colors);

}

Node * readMultiAgentLevel(const string level){
	//std::cerr << "Mutli agent level detected\n";
	std::unordered_map<char, Entity::COLOR> colors;
	std::string line;

	stringstream level_stream(level);

	std::regex color_regex("^[a-z]+:\\s*[0-9A-Z](\\s*,\\s*[0-9A-Z])*\\s*$");
	std::smatch match;

	int matches = 0;
	while (getline (level_stream, line) && line != "" && std::regex_match(line, match, color_regex)) {
		matches++;

		//std::cerr<< line << "\n";
		std::stringstream ss(line);
		std::string color;
		getline(ss, color, ':');

		std::string c;
		while (getline(ss, c, ',')) {
			c.erase(remove(c.begin(), c.end(), ' '), c.end());

			//Transform color to lower case
			//std::transform(color.begin(), color.end(), color.begin(), ::tolower);

			if (color == (std::string("blue"))){
				colors.insert(std::pair<char,Entity::COLOR>(c[0], Entity::BLUE));
			} else if (color == (std::string("red"))){
				colors.insert(std::pair<char,Entity::COLOR>(c[0], Entity::RED));
			} else if (color == (std::string("green"))){
				colors.insert(std::pair<char,Entity::COLOR>(c[0], Entity::GREEN));
			} else if (color == (std::string("cyan"))){
				colors.insert(std::pair<char,Entity::COLOR>(c[0], Entity::CYAN));
			} else if (color == (std::string("magenta"))){
				colors.insert(std::pair<char,Entity::COLOR>(c[0], Entity::MAGENTA));
			} else if (color == (std::string("orange"))){
				colors.insert(std::pair<char,Entity::COLOR>(c[0], Entity::ORANGE));
			} else if (color == (std::string("pink"))){
				colors.insert(std::pair<char,Entity::COLOR>(c[0], Entity::PINK));
			} else if (color == (std::string("yellow"))){
				colors.insert(std::pair<char,Entity::COLOR>(c[0], Entity::YELLOW));
			}
		}
	}


	int agentnum = 0;
	int boxnum = 0;
	for (auto it = colors.begin(); it != colors.end(); ++it) {
		if (isAgent(it->first)) {
			agentnum++;
		}

		if (isBox(it->first)) {
			boxnum++;
		}

	}

	std::list<std::string> rows;

	int cols = 0;
	do
	{

		if (line.length() > cols) {
			cols = line.length();
		}

		rows.push_back(line);
		getline(level_stream, line);
	} while (!level_stream.eof() && (line.length() != 0));



	//std::cerr << "Agents: " << agentnum <<
			//"\nBoxes: " << boxnum <<
	//"\nDim: [" << cols << "," << rows.size() << "]\n";

	return storeInput(rows, cols, colors);
}

Node * setupEnvironment(){
	//std::cerr << "Environment Setup\n";

	string line;
	stringstream ss = stringstream("");

	while(!std::cin.eof() && getline(std::cin, line) && line != ""){
		ss << line << "\n";
	}

	string level_string = ss.str();
	//std::cerr << level_string << "\n";

	std::regex multi_regex("[a-z]+:\\s*[0-9]");
	std::smatch match;

	if (std::regex_search(level_string, match, multi_regex)){
		return readMultiAgentLevel(level_string);
	}else{
		return readSingleAgentLevel(level_string);
	}
}
}
