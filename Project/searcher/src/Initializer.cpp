#include "Initializer.h"
#include "Node.h"
#include <string>
#include <sstream>
#include <list>
#include <unordered_map>
#include <map>
#include <regex>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>

using std::string;
using std::stringstream;
using std::pair;
using std::vector;

//Used for sorting agent array, cause I'm lazy /Martin
bool compAgents (Agent & i,Agent & j) { return (i.getChar() < j.getChar()); }
/*
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
*/

namespace Initializer {

  const char char_free = ' ';
  const char char_wall = '+';
  const char char_static = '-';

  char char_agent(const int agent_num){
    return agent_num + '0';
  }

  bool isAgent(const char c) {
  	return ('0' <= c && c <= '9');
  }

  bool isBox(const char c) {
  	return ('A' <= c && c <= 'Z');
  }

  bool isGoal(const char c) {
  	return ('a' <= c && c <= 'z');
  }

  bool isWall(const char c){
    return c == char_wall;
  }

  bool isFree(const char c){
    return c == char_free;
  }

  void check_unmoveable(vector<string>& rows, std::map<char, string> color_map){
    std::set<string> agent_colors;
    for (auto s : rows)
      for (auto c : s)
        if (isAgent(c))
          agent_colors.insert(color_map[c]);

    for (auto& s : rows)
      for (auto& c : s)
        if (isBox(c) && agent_colors.find(color_map[c])==agent_colors.end())
          c = char_wall;
  }

  Location find_char(const char chr, const vector<string> rows){
    for (int y = 0; y < rows.size(); y++){
      for (int x = 0; x < rows[y].length(); x++){
        if (rows[y][x] == chr)
          return Location(x,y);
      }
    }
    return Location(-1,-1);
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

  vector<vector<string>> split_regions(vector<string> rows, std::map<char, string> color_map){
    bool agent_missing[10];
    for (int i = 0; i < 10; i++){
      agent_missing[i] = true;
    }

    // check for unmoveable objects
    check_unmoveable(rows, color_map);

    int height = rows.size();
    int width = 0;
    for (string s : rows){
      int length = s.length();
      if (length > width)
        width = length;
    }

    vector<char> map(width*height);
    for (int y = 0; y < height; y++){
      for (int x = 0; x < width; x++){
        int i = x + y*width;
        if (x >= rows[y].size()){
          map[i] = char_static;
          continue;
        }
        if (rows[y][x]==char_wall){
          map[i] = char_wall;
        }else{
          map[i] = char_free;
        }

      }
    }

    const vector<Location> neighbors = {Location(1,0),Location(0,1),Location(-1,0),Location(0,-1)};

    for (int agent = 0; agent < 10; agent++){
      std::queue<Location> frontier;
      Location agent_pos = find_char(char_agent(agent), rows);
      if (agent_pos.getX() == -1 && agent_pos.getY() == -1){
        continue;
      }

      // agent already mapped
      if (!isFree(map[agent_pos.getX() + agent_pos.getY()*width])){
        continue;
      }

      map[agent_pos.getX() + agent_pos.getY() * width] = char_agent(agent);
      frontier.push(agent_pos);

      // BFS for each agent to find active regions
      while (!frontier.empty()){
        Location pos = frontier.front();
        frontier.pop();

        for (Location n : neighbors){
          auto tmp = pos+n;
          if (tmp.getX() < 0 || tmp.getX() > width || tmp.getY() < 0 || tmp.getY() > height)
            continue;
          int index = tmp.getX() + tmp.getY() * width;
          if (map[index]==char_free){
            map[index] = char_agent(agent);
            frontier.push(Location(tmp));
          }
        }
      }
    }

    vector<vector<string>> regions;

    for (int a = 0; a < 10; a++){
      vector<string> region;
      int region_size = 0;

      for (int y = 0; y < height; y++){
        stringstream ss("");
        for (int x = 0; x < width; x++){
          if (x > rows[y].size()){
            ss << char_static;
            continue;
          }
          int index = x + y*width;
          char c = map[index];
          if (c == char_agent(a)){
            region_size++;
            ss << rows[y][x];
          }else if (c == char_wall){
            ss << char_wall;
          }else{
            ss << char_free;
          }
        }
        region.push_back(ss.str());
      }
      if (region_size > 0){
        regions.push_back(region);
      }
    }

    // check each region for unmoveable objects
    for (auto& region : regions)
      check_unmoveable(region, color_map);

    return regions;
  }

  bool is_info_string(const string input){
    std::regex multi_regex("[a-z]+:");
    std::smatch match;
    return std::regex_search(input, match, multi_regex);
  }

  string remove_space(const string in){
    string out = in;
    out.erase(remove(out.begin(), out.end(), ' '), out.end());
    return out;
  }

  Entity::COLOR parse_color(const string color){
    if (color == (std::string("blue"))){
      return Entity::BLUE;
    } else if (color == (std::string("red"))){
      return Entity::RED;
    } else if (color == (std::string("green"))){
      return Entity::GREEN;
    } else if (color == (std::string("cyan"))){
      return Entity::CYAN;
    } else if (color == (std::string("magenta"))){
      return Entity::MAGENTA;
    } else if (color == (std::string("orange"))){
      return Entity::ORANGE;
    } else if (color == (std::string("pink"))){
      return Entity::PINK;
    } else if (color == (std::string("yellow"))){
      return Entity::YELLOW;
    }
    return Entity::BLUE;
  }

  std::map<char, string> map_colors(const vector<string> input){
    std::map<char, string> map;

    for (string line : input){
      stringstream ss(remove_space(line));
      string color;
      getline(ss,color,':');

      string c;
      while(!ss.eof()){
        getline(ss, c, ',');
        map[c[0]] = color;
      }
    }
    return map;
  }

  Node* parse_regions(const vector<vector<string>> regions, std::map<char, string> color_map){

    int width = 0;
    int height = 0;
    // find the maximum line width and height
    for (auto& region : regions){
      for (auto& line : region)
        if (line.size() > width)
          width = line.size();
      if (region.size() > height)
        height = region.size();
    }

    Node::maxX = width;
    Node::maxY = height;

    Node::walls.resize(width*height, false);

    vector<Agent> agents;
    vector<Box> boxes;

    for (int r = 0; r < regions.size(); r++){
      auto& region = regions[r];
      for (int y = 0; y < region.size(); y++){
        auto& line = region[y];
        for (int x = 0; x < line.size(); x++){
          char c = line[x];
          if (isWall(c)){
            Node::walls[x + y * width] = true;
          }else if (isAgent(c)){
            agents.emplace_back(c, Location(x, y), parse_color(color_map[c]),r);
          } else if (isBox(c)){
            boxes.emplace_back(c, Location(x, y), parse_color(color_map[c]),r);
          } else if (isGoal(c)){
            Node::goals.emplace_back(c, Location(x, y),r);
          }
        }
      }
    }

    std::sort (agents.begin(), agents.end(), compAgents);

    Node * initialState = new Node();
    initialState->boxes = boxes;
    initialState->agents = agents;

    return initialState;
  }

  Node* read_level_string(const vector<string> lines){

    vector<string> info_strings;
    int i = 0;
    for(;i < lines.size();i++){
      if (!is_info_string(lines[i]))
        break;
      info_strings.push_back(lines[i]);
    }

    // create map of defined colors
    auto color_map = map_colors(info_strings);

    for (auto& c : color_map){
      //std::cerr << c.first << ": " << c.second << std::endl;
    }

    // The rest must be level lines
    vector<string> level_string;
    for (;i<lines.size();i++){
      level_string.push_back(lines[i]);
    }

    // Splitting level into regions
    vector<vector<string>> regions = split_regions(level_string, color_map);
    for (auto& region : regions){
      std::cerr << "Region:\n";
      for (auto& line : region)
        std::cerr << " " << line << std::endl;
    }

    //std::cerr << std::flush;
    // parse all regions to a single node
    return parse_regions(regions, color_map);
  }

  Node* setupEnvironment(){

    vector<string> lines;

    string line;
    while(!std::cin.eof() && getline(std::cin, line) && line != ""){
      lines.push_back(line);
    }
    return read_level_string(lines);
  }
}
