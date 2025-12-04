#include <iostream>
#include <sstream>
#include <fstream>
#include <deque>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <utility>
#include <iterator>
using namespace std;

struct Input {
    vector<vector<string>> patterns;
    vector<vector<string>> outputs;
};

template<typename T>
void pv(const T &t) {
    copy(t.begin(), t.end(), ostream_iterator<typename T::value_type>(cout, ", "));
    cout << endl;
}

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

vector<vector<char>> mappings() {
    vector<vector<char>> v;

    // generate every ordering of the word abcdefg
    // where
    // a b c d e f g
    // 0 1 2 3 4 5 6
    // (ordering so far, remaining chars)
    string abcdefg = "abcdefg";
    vector<char> letters; //(abcdefg.begin(), abcdefg.end());
    deque<char> remaining;
    for (auto &c : abcdefg)
        remaining.push_back(c);

    deque<pair<vector<char>, deque<char>>> q = {make_pair(letters, remaining)};

    while (!q.empty()) {
        pair<vector<char>, deque<char>> p = q.front();
        q.pop_front();

        vector<char> sofar = p.first;
        deque<char> remaining = p.second;

        if (remaining.size() == 0)
            v.push_back(sofar);

        for (int i=0; i < remaining.size(); ++i) {
            vector<char> _sofar = sofar;
            _sofar.push_back(remaining[i]);
            deque<char> _remaining = remaining;
            _remaining.erase(_remaining.begin() + i);
            pair<vector<char>, deque<char>> _p = make_pair(_sofar, _remaining);
            q.push_back(_p);
        }
    }

    return v;
}

bool isValidMapping(vector<char> &m, vector<string> patterns) {
    map<vector<int>, int> segmentsToNum = {
        {{0, 1, 2, 4, 5, 6}, 0},
        {{2, 5}, 1},
        {{0, 2, 3, 4, 6}, 2},
        {{0, 2, 3, 5, 6}, 3},
        {{1, 2, 3, 5}, 4},
        {{0, 1, 3, 5, 6}, 5},
        {{0, 1, 3, 4, 5, 6}, 6},
        {{0, 2, 5}, 7},
        {{0, 1, 2, 3, 4, 5, 6}, 8},
        {{0, 1, 2, 3, 5, 6}, 9}
    };

    for (string &pattern : patterns) {
        // e.g. 01236 -> 9
        vector<int> segments;
        for (char &c : pattern) { // const char &c?
            vector<char>::iterator it = find(m.begin(), m.end(), c);
            segments.push_back((int)(it - m.begin())); // TODO this find is not necessary
        }
        sort(segments.begin(), segments.end()); // TODO getting worse
        if (segmentsToNum.count(segments) > 0) {
            segmentsToNum.erase(segments);
        } else {
            return false;
        }
    }

    return true;
}

vector<int> toDigits(vector<char> &m, vector<string> strs) {
    map<vector<int>, int> segmentsToNum = {
        {{0, 1, 2, 4, 5, 6}, 0},
        {{2, 5}, 1},
        {{0, 2, 3, 4, 6}, 2},
        {{0, 2, 3, 5, 6}, 3},
        {{1, 2, 3, 5}, 4},
        {{0, 1, 3, 5, 6}, 5},
        {{0, 1, 3, 4, 5, 6}, 6},
        {{0, 2, 5}, 7},
        {{0, 1, 2, 3, 4, 5, 6}, 8},
        {{0, 1, 2, 3, 5, 6}, 9}
    };

    vector<int> v;
    for (auto str : strs) {
        vector<int> segments;
        for (char c : str) {
            vector<char>::iterator it = find(m.begin(), m.end(), c);
            segments.push_back((int)(it - m.begin()));
        }
        sort(segments.begin(), segments.end());
        int digit = segmentsToNum[segments];
        v.push_back(digit);
    }
    return v;
}

int partTwo(struct Input &input) {
    // for each segment, keep list of candidate values it could be
    // then eliminate invalid candidates for each segment
    // until only candidate remains per segment?

    auto ms = mappings();
    int out = 0;

    for (int i=0; i < input.patterns.size(); ++i) {
        int cur = 0;
        auto pattern = input.patterns[i];
        auto output = input.outputs[i];
        for (vector<char> m : ms) {
            if (isValidMapping(m, pattern)) {
                for (auto digit : toDigits(m, output)) {
                    cur *= 10;
                    cur += digit;
                }
                continue;
            }
        }
        out += cur;
    }
    return out;
}

int main(int argc, char **argv) {
    Input input = vread(argv[1]);
    // cout << partOne(input.outputs) << endl;

    // vector<char> v = {'d','e','a','f','g','b','c'};
    // vector<string> _v = {"cdfeb", "fcadb", "cdfeb", "cdbaf"};
    // cout << isValidMapping(v, input.patterns[0]) << endl;
    // pv(toDigits(v, _v));
    // cout << endl;

    cout << partTwo(input) << endl; 
    return 0;
}