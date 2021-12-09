#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
using namespace std;

struct Input {
    vector<vector<string>> patterns;
    vector<vector<string>> outputs;
};

vector<string> stov(string &input) {
    vector<string> vec;
    string substr;
    while (input.size() > 0) {
        int space_index = input.find(' ');
        if (space_index == string::npos) {
            substr = input;
            input.erase(input.begin(), input.end());
        } else {
            substr = input.substr(0, space_index);
            input.erase(0, space_index+1);
        }
        vec.push_back(substr);
    }
    return vec;
}

Input vread(const string &filename) {
    ifstream file;
    file.open(filename);
    string line;
    vector<vector<string>> patterns;
    vector<vector<string>> outputs;
    
    while (getline(file, line)) {
        int pipeIndex = line.find('|');
        string patternsStr = line.substr(0, pipeIndex-1);
        line.erase(0, pipeIndex + 2);

        patterns.push_back(stov(patternsStr));
        outputs.push_back(stov(line));
    }

    struct Input input;
    input.patterns = patterns;
    input.outputs = outputs;
    return input;
}

int partOne(vector<vector<string>> &digits) {
    // map from digit -> # segments
    map<int, int> m = {
        {1, 2},
        {2, 5},
        {3, 5},
        {4, 4},
        {5, 5},
        {6, 6},
        {7, 3},
        {8, 7},
        {9, 6},
        {0, 6},
    };

    int ntimes = 0;
    for (vector<string> seq : digits) {
        for (string s : seq) {
            int len = s.length();
            if (len == 2 || len == 4 || len == 3 || len == 7)
                ++ntimes;
        }
    }
    return ntimes;
}

int main(int argc, char **argv) {
    Input input = vread(argv[1]);
    cout << partOne(input.outputs) << endl;
    return 0;
}