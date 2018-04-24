#include "Initializer.h"
#include "Node.h"
#include <string>
#include <list>
#include <unordered_map>
#include <regex>
#include <iostream>
#include <iterator>
#include <algorithm>

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
    //std::vector<Goal> goals = std::vector<Goal>();

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
          agents.emplace_back(chr, pair<int, int>(x, y), colors.find(chr)->second);
        } else if (isBox(chr)){
          boxes.emplace_back(chr, pair<int, int>(x, y), colors.find(chr)->second);
        } else if (isGoal(chr)){
          std::cerr << "Goal is found: " << chr << "\n";
          Node::goals.emplace_back(chr, pair<int, int>(x, y));
        } else if (chr == ' '){
          //Do nothing, as nothing is printed
        }
      }
    }
//    Node::goals = goals;

    initialState->boxes = boxes;
    initialState->agents = agents;
    /*
    Everything is stored, so we print the state as a sanity check for the user
    */
    cerr << initialState->toString();
    return initialState;
  }



  Node * setupEnvironment(){
    std::cerr << "HI!\n";
    std::unordered_map<char, Entity::COLOR> colors;
    std::string line;

    std::regex color_regex("^[a-z]+:\\s*[0-9A-Z](\\s*,\\s*[0-9A-Z])*\\s*$");
    std::smatch match;

    std::string firstline = std::string("red: 0,A");

    while (getline (std::cin, line) && line != "" && std::regex_match(line, match, color_regex)) {

      std::cerr<< line << "\n";
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
    int goalnum = 0;
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
