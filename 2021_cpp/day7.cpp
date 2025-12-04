#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <vector>

using namespace std;

vector<int> vstream(const string &filename) {
    ifstream file;
    file.open(filename);
    stringstream ss;
    vector<int> vec;

    ss << file.rdbuf();
    while (ss.good()) {
        string substr;
        getline(ss, substr, ',');
        vec.push_back(stoi(substr));
    }
    return vec;
}

int partOne(vector<int> &nums) {
    int mindist = numeric_limits<int>::max();
    int maxnum = *max_element(nums.begin(), nums.end());

    for (int location=0; location < maxnum; ++location) {
        int sumdist = 0;
        for (auto num : nums) {
            int dist = abs(location - num);
            sumdist += dist;
        }
        mindist = min(mindist, sumdist);
    }

    return mindist;
    //sort(nums.begin(), nums.end());
    //return nums[int(nums.size()/2)];
}

int distance(int x, int target) {
    int sumd = 0;
    for (int d=0; d < abs(target-x); ++d) {
        sumd += (d+1);
    }
    return sumd;
}

int partTwo(vector<int> &nums) {
    int mindist = numeric_limits<int>::max();
    int maxnum = *max_element(nums.begin(), nums.end());

    for (int location=0; location <= maxnum; ++location) {
        int sumdist = 0;
        for (auto num : nums) {
            sumdist += distance(num, location);
        }
        mindist = min(mindist, sumdist);
    }
    return mindist;
}

int main(int argc, char **argv) {
    vector<int> vec = vstream(argv[1]);
    cout << partOne(vec) << endl;
    cout << partTwo(vec) << endl;
}