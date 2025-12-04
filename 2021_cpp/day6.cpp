#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>
#include <assert.h>

using namespace std;

vector<int> vread(const string &filename) {
    ifstream file;
    file.open(filename);
    string line;
    getline(file, line);
    vector<int> buf;

    while (line.length() > 0) {
        int commaIndex = line.find(',');
        if (commaIndex == string::npos) {
            commaIndex = line.length() + 1;
        }

        int num = stoi(line.substr(0, commaIndex));
        line.erase(0, commaIndex+1);
        buf.push_back(num);
    }
    return buf;
}

vector<int> vstream(const string &filename) {
    ifstream file;
    file.open(filename);
    stringstream buf;
    vector<int> vec;
    buf << file.rdbuf(); // stream must be an lvalue

    while(buf.good()) {
        string substr;
        getline(buf, substr, ',');
        vec.push_back(stoi(substr));
    }

    file.close();
    return vec;
}

int partOne(vector<int> fish, int ndays) {
    for (int day=0; day < ndays; ++day) {
        vector<int> extension;
        for (vector<int>::iterator it = fish.begin(); it < fish.end(); ++it) {
            if (*it == 0) {
                extension.push_back(8);
                (*it) = 6;
            } else {
                --(*it);
            }
        }
        fish.insert(fish.end(), extension.begin(), extension.end());
    }
    return fish.size();
}

long partTwo(vector<int> init, int ndays) {

    map<int, long> fish;
    for (int i=0; i < 9; ++i)
        fish[i] = 0L;
    
    for (int lifespan : init) {
        fish[lifespan] += 1;
    }

    for (int day=0; day < ndays; ++day) {
        long spawned = fish[0];
        for (int i=1; i < 9; ++i) {
            fish[i-1] = fish[i];
        }
        fish[8] = spawned;
        fish[6] += spawned;
    }

    long totalFish = 0;
    for (pair<int, long> p : fish)
        totalFish += p.second;
    return totalFish;
}

int main(int argc, char **argv) {
    vector<int> buf = vstream(argv[1]);
    cout << partTwo(buf, stoi(argv[2])) << endl;
}