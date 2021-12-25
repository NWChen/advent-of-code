#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

vector<int> vread(const string& path) {
  ifstream file(path);
  vector<int> nums;
  int num;

  while (file >> num) {
  	nums.push_back(num);
  }
  return nums;
}

int n_incr(vector<int>& buf) {
  int out = 0;
  int prev;
  for (int i=1; i < buf.size(); ++i) {
  	prev = buf[i-1];
  	if (buf[i] > prev) {
  	  out += 1;
	}
  }
  return out;
}

int main(int argc, char *argv[]) {
  vector<int> buf = vread(argv[1]);
  int out = n_incr(buf);
  cout << out << endl;
  return 0;
}
