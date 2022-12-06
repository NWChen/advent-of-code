#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "day5utils.h"
using namespace std;

Point csvToPoint(const string &csv) {
    int commaIndex = csv.find(',');
    string left = csv.substr(0, commaIndex);
    csv.erase(0, commaIndex+1);
    string right = move(csv);

    Point point = { stoi(left), stoi(right) };
    return point;
}

Input readInput(const string& filename) {
    ifstream file;    
    string line;
    vector<pair<Point, Point> > buf;
    file.open(filename);

    int x_max = 0, y_max = 0;
    while (getline(file, line)) {
        int arrowIndex = line.find(" -> ");
        string left = line.substr(0, arrowIndex);
        line.erase(0, arrowIndex + 4);
        string right = move(line);

        Point leftPt = csvToPoint(left);
        Point rightPt = csvToPoint(right);
        buf.push_back(make_pair(leftPt, rightPt));

        x_max = max(x_max, max(leftPt.x, rightPt.x));
        y_max = max(y_max, max(leftPt.y, rightPt.y));
    } 

    struct Input input = {
        x_max,
        y_max,
        buf
    };
    return input;
}
