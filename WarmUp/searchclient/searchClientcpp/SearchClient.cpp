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
	string s(&c);
	std::smatch match;
	return std::regex_match(s, match, std::regex("[0-9]"));
}

bool isBox(char c) {
	string s(&c);
	std::smatch match;
	return std::regex_match(s, match, std::regex("[A-Z]"));
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

	for (string row : rows) {
		std::cout << row << "\n";
	}




		

}


SearchClient::~SearchClient()
{
}