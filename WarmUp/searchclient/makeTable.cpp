#include <iostream>       // std::cout
#include <string>         // std::string
#include <sstream>
#include <fstream>

void readAlgorithm(std::string str, int i);
void readTimer(std::string str, int i);
void readLength(std::string str, int i);
void readGenerated(std::string str, int i);
void readMemory(std::string str, int i);
//std::string noPeriods(sdt::string str);
bool isFinished(std::string str);

void printResults();

std::string timer[12];
std::string memory[12];
std::string lengthsol[12];
std::string generated[12];
std::string level[6] = {"SAD1","SAD2","friendsofDFS","friendsofBFS","SAFirefly","SACrunch"};
std::string algorithm[12];

int main ()
{
int i;
for (i = 0; i < 12; i++){
	//Read file into string
	std::stringstream ss;
	ss << "log" << i << ".txt";
	std::string filename = ss.str();
	std::ifstream t (filename);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string str = buffer.str();
	//Work with string
//	std::size_t found = str.find("Generated");
//	if (found!=std::string::npos)
//		std::cout << "first 'generated' found at: " << found << '\n';


	readAlgorithm(str, i);
	readTimer(str, i);
	readLength(str, i);
	readGenerated(str, i);
	readMemory(str, i);
}

printResults();

  // different member versions of find in the same order as above:
//  std::size_t found = str.find("Generated");
//  if (found!=std::string::npos)
//    std::cout << "first 'generated' found at: " << found << '\n';

/*  found=str.find("needles are small",found+1,6);
  if (found!=std::string::npos)
    std::cout << "second 'needle' found at: " << found << '\n';

  found=str.find("haystack");
  if (found!=std::string::npos)
    std::cout << "'haystack' also found at: " << found << '\n';

  found=str.find('.');
  if (found!=std::string::npos)
    std::cout << "Period found at: " << found << '\n';

  // let's replace the first needle:
  str.replace(str.find(str2),str2.length(),"preposition");
  std::cout << str << '\n';*/

  return 0;
}


void readAlgorithm(std::string str, int i){

	std::size_t location = str.rfind("Search starting with strategy ");
	//Needs to find area after "strategy" and before "."
	std::size_t startLocation = location + std::string("Search starting with strategy ").size();
	std::size_t locationOfDot = str.find(".", startLocation);
	std::size_t length = locationOfDot - startLocation;
	algorithm[i] = str.substr(startLocation, length);
}
void readTimer(std::string str, int i){
	std::size_t location = str.rfind("Time: ");
	std::size_t locationOfs = str.find("s", location);

	//Needs to find area after "strategy" and before "."
	std::size_t startLocation = location + std::string("Time: ").size();
	std::size_t length = locationOfs - startLocation + 1;
	//Find timestring
	std::string time = str.substr(startLocation, length);
	//Parse as double
	timer[i] = time;
}

void readMemory(std::string str, int i){
	std::size_t location = str.rfind("Used: ");
	std::size_t locationOfs = str.find("MB", location);

	//Needs to find area after "strategy" and before "."
	std::size_t startLocation = location + std::string("used: ").size();
	std::size_t length = - startLocation + locationOfs + 2;
	//Find timestring
	std::string time = str.substr(startLocation, length);
	memory[i] = time;
}


void readLength(std::string str, int i){
	if (!isFinished(str)){
		lengthsol[i] = "No solution";
		return;
	}
	std::size_t location = str.rfind("length ");
	std::size_t locationOfnew = str.find("\n", location);

	//Needs to find area after "strategy" and before "."
	std::size_t startLocation = location + std::string("length ").size();
	std::size_t length = - startLocation + locationOfnew;
	std::string l = str.substr(startLocation, length);

	lengthsol[i] = l;
}
void readGenerated(std::string str, int i){
	std::size_t location = str.rfind("Generated:  ");
	std::size_t locationOfDot = str.find(",", location);

	//Needs to find area after "strategy" and before "."
	std::size_t startLocation = location + std::string("Generated:  ").size();
	std::size_t length = - startLocation + locationOfDot;
	std::string g = str.substr(startLocation, length);
//	std::string news = noPeriods(l);
	//Parse as int
	generated[i] = g;
}

bool isFinished(std::string str){
	std::size_t found = str.find("Found solution of length ");
	if (found!=std::string::npos)
		return true;
	return false;

}

void printResults(){
std::string s;
s.append("\\begin{tabular}{|l|l|r|r|r|r|}\n");
s.append("\\hline\n");
s.append("Level & Client & Time & Memory & Solution Length & Nodes Generated\\\\");
s.append("\\hline\n");

int i;
for (i = 0; i < 12; i++){
	s.append(level[i/2]);
	s.append(" & ");
	s.append(algorithm[i]);
	s.append(" & ");
	s.append(timer[i]);
	s.append(" & ");
	s.append(memory[i]);
	s.append(" & ");
	s.append(lengthsol[i]);
	s.append(" & ");
	s.append(generated[i]);
	s.append("\\\\  \\hline\n");

}
s.append("\\end{tabular}\n");
//Print string to file


std::ofstream out("output.txt");
out << s;
out.close();

}
