#include "Initializer.h"
#include "Node.h"
#include <string>
#include <list>
#include <unordered_map>
#include <regex>
#include <iostream>
#include <iterator>


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




  Node * storeInput(std::list<std::string> rows, int cols, std::unordered_map<char, std::string> colors){
    using namespace std;

    std::vector<Box> boxes = std::vector<Box>();
    std::vector<Agent> agents = std::vector<Agent>();
    std::vector<Goal> goals = std::vector<Goal>();

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
          agents.emplace_back((int)(chr - '0'), pair<int, int>(x, y));
        } else if (isBox(chr)){
          boxes.emplace_back(chr, pair<int, int>(x, y));
        } else if (isGoal(chr)){
          goals.emplace_back(chr, pair<int, int>(x, y));
        } else if (chr == ' '){
          //Do nothing, as nothing is printed
        }
      }
    }
    Node::goals = goals;
    initialState->boxes = boxes;
    initialState->agents = agents;
    /*
    Everything is stored, so we print the state as a sanity check for the user
    */
    cerr << initialState->toString();

  }



  Node * setupEnvironment(){
    std::unordered_map<char, std::string> colors;
    std::string line;

    std::regex color_regex("^[a-z]+:\\s*[0-9A-Z](\\s*,\\s*[0-9A-Z])*\\s*$");
    std::smatch match;


    while (getline (std::cin, line) && !line.compare("") && std::regex_match(line, match, color_regex)) {

      printf("%s\n", line.c_str());
      std::stringstream ss(line);
      std::string color;
      getline(ss, color, ':');

      std::string c;
      while (getline(ss, c, ',')) {
        c.erase(remove(c.begin(), c.end(), ' '), c.end());
        colors.insert(std::pair<char,std::string>(c.c_str()[0], color));
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
      getline(std::cin, line);
    } while (!std::cin.eof() && (line.length() != 0));
    std::cerr << "Agents: " << agentnum <<
    "\nBoxes: " << boxnum <<
    "\nDim: [" << cols << "," << rows.size() << "]\n";

    return storeInput(rows, cols, colors);
  }
}
