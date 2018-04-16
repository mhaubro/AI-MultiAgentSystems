#include <thread>
#include <chrono>
#include <iostream>       // std::cout
#include <string>         // std::string
#include <sstream>
#include <fstream>


bool probe(int i);

//Takes two arguments: Timeout and index. Probes roughly once a second, for timeout seconds.
int main(int argc, char** argv){
if (argc != 3)
	return 1;

int index = std::stoi(argv[2]);
int timeout = std::stoi(argv[1]);

int time = 0;
while(time < timeout){
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	if (probe(index))
		return 0;
	time++;
}

return 0;
}

bool probe(int i){
//Loading string
	std::stringstream ss;
	ss << "log" << i << ".txt";
	std::string filename = ss.str();
	std::ifstream t (filename);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string str = buffer.str();
//Probing string
	std::size_t found = str.find("Found solution of length ");
	if (found!=std::string::npos)
		return true;
	return false;
}

