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
#include <vector>
#include <queue>

using std::string;
using std::stringstream;
using std::pair;
using std::vector;

//Used for sorting agent array, cause I'm lazy /Martin
bool compAgents (Agent & i,Agent & j) { return (i.chr<j.chr); }

class Location{
public:

  int x;
  int y;

  Location():
  x(0), y(0)
  {
  }

  Location(const int _x, const int _y):
  x(_x), y(_y)
  {
  }

  Location(const Location& location):
  x(location.x), y(location.y)
  {
  }

  //Location& operator=(const Location& b){
  //  return Location(b);
  //}

  Location operator+(const Location &b){
    return Location(this->x+b.x, this->y+b.y);
  }

}; 

std::ostream &operator<< (std::ostream &os, Location const& location){
  os << "[" << location.x << "," << location.y << "]";
  return os;
}

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
        	Agent a = Agent(chr, pair<int, int>(x, y), colors.find(chr)->second);

          agents.emplace_back(&a);
        } else if (isBox(chr)){
          boxes.emplace_back(chr, pair<int, int>(x, y), colors.find(chr)->second);
        } else if (isGoal(chr)){
          //std::cerr << "Goal is found: " << chr << "\n";
          Node::goals.emplace_back(chr, pair<int, int>(x, y));
        } else if (chr == ' '){
          //Do nothing, as nothing is printed
        }
      }
    }
//    Node::goals = goals;
    std::sort (agents.begin(), agents.end(), compAgents);
    initialState->boxes = boxes;
    initialState->agents = agents;
    /*
    Everything is stored, so we print the state as a sanity check for the user
    */
    cerr << initialState->toString();
    ////std::cerr << "Hi\n";
    return initialState;
  }

  Location find_char(const char chr, const string level){
    stringstream ss(level);

    int y = 0;
    while(true){
      string line;
      getline(ss, line);

      for (int x = 0; x < line.length(); x++){
        if (line[x] == chr)
          return Location(x,y);
      }
      if (ss.eof())
        return Location(-1,-1);
      y++;
    }

  }

  vector<string> split_lines(const string s){
    vector<string> lines;
    stringstream ss(s);

    while(!ss.eof()){
      string line;
      getline(ss, line);
      lines.push_back(line);
    }
    return lines;
  }

  string map_to_string(vector<char> map, int width, int height){
    stringstream ss("");

    for (int y = 0; y < height; y++){
      for (int x = 0; x < width; x++){
        char c = map[x + y*width];
        ss << c;
      }
      ss << std::endl;
    }
    return ss.str();
  }

  

  vector<string> split_regions(string level){
    const string region_test_string = "+++++++++++++++++++++++++++++\n+B b                       A+\n+                           +\n+   a   0                   +\n+++++++++++++++++++++++++++++\n+   A                   a  B+\n+                           +\n+                           +\n+       1                  b+\n+++++++++++++++++++++++++++++";
    // remove when testing done
    level = region_test_string;
    std::cerr << "region_test_string = \n" << region_test_string << std::endl;


    bool agent_missing[10];
    for (int i = 0; i < 10; i++){
      agent_missing[i] = true;
    }

    std::cerr << "splitting regions\n" << std::flush;
    vector<string> rows = split_lines(level);
    int height = rows.size();
    int width = 0;
    std::cerr << "width: " << width << " height: " << height << std::endl << std::flush;
    for (string s : rows){
      int length = s.length();
      if (length > width)
        width = length;
    }

    std::cerr << "Creating Map:\n" << std::flush;
    vector<char> map(width*height);
    for (int y = 0; y < height; y++){
      for (int x = 0; x < width; x++){
        int i = x + y*width;
        if (x >= rows[y].size()){
          map[i] = '+';
          continue;
        }
        if (rows[y][x]=='+'){
          map[i] = '+';
        }else{
          map[i] = ' ';
        }

      }
    }

    std::cerr << map_to_string(map, width, height) << std::endl;

    std::cerr << "splitting regions\n" << std::flush;
    const vector<Location> neighbors = {Location(1,0),Location(0,1),Location(-1,0),Location(0,-1)};

    
    for (int agent = 0; agent < 10; agent++){
      std::queue<Location> frontier;
      Location agent_pos = find_char(agent + '0', level);
      if (agent_pos.x == -1 && agent_pos.y == -1){
        continue;
      }
      std::cerr << "Found agents at: " << agent_pos << std::endl;
      map[agent_pos.x + agent_pos.y * width] = '0' + agent;
      frontier.push(agent_pos);

      // bredde først
      while (!frontier.empty()){
        Location pos = frontier.front();
        frontier.pop();
        
        for (Location n : neighbors){
          auto tmp = pos+n;
          if (tmp.x < 0 || tmp.x > width || tmp.y < 0 || tmp.y > height)
            continue;
          int index = tmp.x + tmp.y * width;
          if (map[index]==' '){
            map[index] = '0'+agent;
            frontier.push(Location(tmp));
          }
        }
      }
    }

    std::cerr << map_to_string(map, width, height) << std::endl << std::flush;

    vector<string> regions = vector<string>();

    for (int a = 0; a < 10; a++){
      stringstream ss("");
      int region_size = 0;

      for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
          int index = x + y*width;
          if (map[index] == '0' + a && x < rows[y].size()){
            region_size++;
            ss << rows[y][x];
          }else {
            ss << '+';
          }
        }
        ss << std::endl;
      }

      if (region_size > 0){
        regions.push_back(ss.str());
      }
     
    }
    
    for (int i = 0; i < regions.size(); i++){
       std::cerr << "Region: " << i << std::endl << regions[i];
    }

    return regions;
  }
  

  Node * readSingleAgentLevel(const string level){

    //std::cerr << "Single agent level detected\n";
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

    std::cerr << "Agents: " << agentnum <<
    "\nBoxes: " << boxnum <<
    "\nDim: [" << cols << "," << rows.size() << "]\n";

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

    split_regions(level_string);

    if (std::regex_search(level_string, match, multi_regex)){
      return readMultiAgentLevel(level_string);
    }else{
      return readSingleAgentLevel(level_string);
    }
  }
}
