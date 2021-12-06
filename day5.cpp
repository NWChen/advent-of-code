#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "day5utils.h"
using namespace std;

// number of line overlaps
int partOne(struct Input &input) {
    int numOverlaps = 0;
    vector<vector<int>> grid(
        input.y_max + 1, // increment because this is size, *_max is 0-indexed
        vector<int> (input.x_max + 1)
    );

    for (pair<Point, Point> line : input.lines) {
        Point left, right;
        left = line.first;
        right = line.second;

        // vertical line
        if (left.x == right.x) {
            int min_y = min(left.y, right.y);
            int max_y = max(left.y, right.y);
            for (int _y = min_y; _y <= max_y; ++_y) {
                grid[_y][left.x] += 1;
                if (grid[_y][left.x] == 2)
                    ++numOverlaps;
            }
        }

        // horizontal line
        else if (left.y == right.y) {
            int min_x = min(left.x, right.x);
            int max_x = max(left.x, right.x);
            for (int _x = min_x; _x <= max_x; ++_x) {
                grid[left.y][_x] += 1;
                if (grid[left.y][_x] == 2)
                    ++numOverlaps;
            }
        }
    }
    return numOverlaps;
}

int partTwo(Input &input) {
    int numOverlaps = 0;
    vector<vector<int>> grid(
        input.y_max + 1,
        vector<int> (input.x_max + 1)
    );
    cout << grid.size() << ", " << grid[0].size() << endl;

    for (pair<Point, Point> points : input.lines) {
        Point left = points.first;
        Point right = points.second;
        int dx = right.x - left.x; //left.x - right.x;
        if (dx != 0)
            dx /= abs(dx);
        int dy = right.y - left.y; //left.y - right.y;
        if (dy != 0)
            dy /= abs(dy);
        int _x = left.x, _y = left.y;

        // for every point from left -> right, move to (x+dx, y+dy)
        grid[_y][_x] += 1;
        if (grid[_y][_x] == 2)
            ++numOverlaps;
        while (abs(_x) <= abs(right.x) && abs(_y) <= abs(right.y)) {
            _x += dx;
            _y += dy;
            grid[_y][_x] += 1;
            if (grid[_y][_x] == 2)
                ++numOverlaps;
            cout << _x << ", " << _y << " : " << left.x << "," << left.y << " - " << right.x << "," << right.y << endl;
        }
    }
    return numOverlaps;
}

int main(int argc, char **argv) {
    struct Input input = readInput("input5-1.txt");

    int partOneAns = partOne(input);
    cout << partOneAns << endl;

    int partTwoAns = partTwo(input);
    cout << partTwoAns << endl;
    // cout << input.lines.size() << endl;
    // for (auto line : input.lines) {
        // cout << line.first.x << ", " << line.first.y << " -> ";
        // cout << line.second.x << ", " << line.second.y << endl;
    // }
    // cout << input.x_max << endl;
    // cout << input.y_max << endl;
    return 0;
}