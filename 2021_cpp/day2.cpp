#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <tuple>
#include <vector>
using namespace std;

void pv(vector<tuple<string, int>>& vec) {
  string direction;
  int amount;

  for (tuple<string, int> tup : vec) {
  	tie(direction, amount) = tup;
  	cout << direction << " " << amount << endl;
  }	
}

void vread(const string& path, vector<tuple<string, int>>& buf) {
  ifstream file(path); 
  string line, direction;
  int amount;
  
  while (getline(file, line)) {
	stringstream(line) >> direction >> amount;
  	buf.push_back(make_tuple(direction, amount));
  }
}

int main(int argc, char *argv[]) {
  string filename = argv[1];
  vector<tuple<string, int>> buf;
  vread(filename, buf);
 
  string direction;
  int amount = 0, distance = 0, depth = 0;
  for (tuple<string, int> tup : buf) {
  	tie(direction, amount) = tup;
	
	if (direction == "forward") {
	  distance += amount;
	} else if (direction == "up") {
	  depth -= amount;
	} else if (direction == "down") {
	  depth += amount;
	}
  }

  cout << distance * depth << endl;

  int aim = 0;
  depth = 0;
  distance = 0;

  for (tuple<string, int> tup : buf) {
  	tie(direction, amount) = tup;
  	if (direction == "forward") {
  	  distance += amount;
  	  depth += (aim * amount);
	} else if (direction == "up") {
	  aim -= amount;
	} else if (direction == "down") {
	  aim += amount;
	}
  }

  cout << distance * depth << endl;

  return 0;
}
