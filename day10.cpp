#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
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

char firstIllegalChar(vector<char> &row) {
    map<char, char> m;
    m[')'] = '(';
    m[']'] = '[';
    m['}'] = '{';
    m['>'] = '<';

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
    return 0;
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

int main(int argc, char **argv) {
    vector<vector<char> > rows = read(argv[1]);
    cout << partOne(rows) << endl;
    return 0;
}
