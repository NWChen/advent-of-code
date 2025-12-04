#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

// pass path by reference
inline string read(const string& path) {
  ostringstream buf;
  ifstream input(path); // c_str()
  buf << input.rdbuf();
  return buf.str();
}

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
  int out = n_incr(buf);
  //int out = triple_incr(buf);
  cout << out << endl;
  return 0;
}
