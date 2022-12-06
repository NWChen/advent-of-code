#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

struct Point {
    int x;
    int y;
};

struct Input {
    int x_max;
    int y_max;
    vector<pair<Point, Point> > lines;
};

Input readInput(const string& filename);
