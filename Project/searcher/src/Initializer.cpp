#include "Initializer.h"

namespace Initializer {

  void storeInput(){

  }

  void setupEnvironment(){
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

    list<string> rows;

    int cols = 0;
    do
    {

      if (line.length() > cols) {
        cols = line.length();
      }

      rows.push_back(line);
      getline(std::cin, line);
    } while (!std::cin.eof() && (line.length() != 0));

  }
}
