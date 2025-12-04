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

int triple_incr(vector<int>& buf) {
  int last_sum = 0;
  int cursum = buf[0] + buf[1] + buf[2];
  int out = 0; 
  
  for (int i=0; i < buf.size()-2; ++i) {
  	last_sum = cursum;
	cursum -= buf[i];
	cursum += buf[i+3];
	if (cursum > last_sum) {
	  ++out;
	}
  }
  return out;
}

int main(int argc, char *argv[]) {
  vector<int> buf = vread(argv[1]);
  int out = triple_incr(buf);
  cout << out << endl;
  return 0;
}
