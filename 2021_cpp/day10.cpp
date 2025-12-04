#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <stack>
#include <utility>
#include "utils.h"
using namespace std;

vector<vector<char> > read(const string &filename) {
    ifstream file;
    file.open(filename);
    string line;
    vector<vector<char> > out;

    while (getline(file, line)) {
        vector<char> row;
        for (auto &c : line)
            row.push_back(c); 
        out.push_back(row);
    }
    return out;
}

map<char, char> getMap() {
    map<char, char> m;
    m[')'] = '(';
    m[']'] = '[';
    m['}'] = '{';
    m['>'] = '<';
    return m;
}

char firstIllegalChar(vector<char> &row) {
    map<char, char> m = getMap();
    stack<char> stk;
    for (char &c : row) {
        if (m.count(c) && stk.size()) {
            if (stk.top() != m[c])
                return c;
            stk.pop();
        } else {
            stk.push(c);
        }
    }
    return '0';
}

int partOne(vector<vector<char> > &rows) {
    map<char, int> points;
    points[')'] = 3;
    points[']'] = 57;
    points['}'] = 1197;
    points['>'] = 25137;

    int score = 0;
    for (auto &row : rows) {
        char res = firstIllegalChar(row);
        if (res) // corrupt line
            score += points[res];
    }
    return score;
}

// returns the vector of chars required to complete a string
vector<char> complete(vector<char> &row) {
    vector<char> out;
    map<char, char> m = getMap();
    deque<char> stk;
    for (char &c : row) {
        if (m.count(c) && stk.size() && stk.back() == m[c]) { 
            stk.pop_back();
        } else {
            stk.push_back(c);
        }
    }

    map<char, char> revm;
    revm['('] = ')';
    revm['['] = ']';
    revm['{'] = '}';
    revm['<'] = '>';
    for (auto it = stk.rbegin(); it != stk.rend(); ++it)
        out.push_back(revm[*it]);

    return out;
}

long score(vector<char> v) {
    map<char, int> points;
    points[')'] = 1;
    points[']'] = 2;
    points['}'] = 3;
    points['>'] = 4;

    long score = 0;
    for (auto &c : v) {
        score *= 5;
        score += points[c];
    }
    return score;
}

// TODO this shouldn't be needed, but you fucked up so here we are
bool isImbalanced(vector<char> &row) {
    map<char, char> m = getMap();
    stack<char> stk;

    for (char &c : row) {
        if (stk.size() && m.count(c)) {
            if (stk.top() == m[c])
                stk.pop();
            else
                return false;
        } else {
            stk.push(c);
        }
    }

    return stk.size() > 0;
}

long partTwo(vector<vector<char> > & rows) {
    vector<long> scores;
    for (vector<char> &row : rows) {
        if (firstIllegalChar(row) == '0') {
            long s = score(complete(row));
            scores.push_back(s);
        }
    }
    sort(scores.begin(), scores.end());
    int mid_i = scores.size()/2;
    return scores.at(mid_i);
}

int main(int argc, char **argv) {
    vector<vector<char> > rows = read(argv[1]);
    cout << partOne(rows) << endl;
    cout << partTwo(rows) << endl;
    return 0;
}
