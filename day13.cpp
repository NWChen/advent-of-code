#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
using namespace std;

struct Point {
    int x;
    int y;
};

struct Fold {
    string dim; 
    int value;
};

struct Input {
    vector<Point> points;
    vector<Fold> folds;
};

shared_ptr<Input> read(const string &filename) {
    vector<Point> v;
    vector<Fold> f;
    ifstream file;
    file.open(filename);
    string line, x, y;
    struct Input input;

    while (getline(file, line)) {
        if (line.size() && line[0] != 'f') {
            int commaIndex = line.find(',');
            x = line.substr(0, commaIndex);
            y = line.substr(commaIndex+1, line.length());
            v.push_back({stoi(x), stoi(y)});
        } else {
            int eqIndex = line.find('=');
            if (eqIndex != string::npos) {
                string dim = line.substr(eqIndex-1, eqIndex);
                string value = line.substr(eqIndex+1, line.length());
                f.push_back({dim, stoi(value)});
            }
        }
    }

    input.points = v;
    input.folds = f;
    return make_shared<Input>(input);
}

int main(int argc, char **argv) {
    shared_ptr<Input> input = read(argv[1]);
    for (auto &pt : input->points) {
        cout << pt.x << "," << pt.y << endl;
    }

    for (auto &fold : input->folds) {
        cout << fold.dim << "=" << fold.value << endl;
    }
}