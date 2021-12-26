#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <limits>
using namespace std;


struct Frame {
    int x;
    int y;
    int dx;
    int dy;
};

struct Input {
    int xmin;
    int xmax;
    int ymin;
    int ymax;
};

Input fread(const string &filename) {
    unsigned xmin, xmax, ymin, ymax;
    ifstream file;
    file.open(filename);
    string line;
    getline(file, line);

    const regex expr("^target area: x=(-*[0-9]+)..(-*[0-9]+), y=(-*[0-9]+)..(-*[0-9]+)");
    smatch match;
    if (regex_search(line, match, expr)) {
        struct Input input = { 
            stoi(match[1]),
            stoi(match[2]),
            stoi(match[3]),
            stoi(match[4])
        };
        return input;
    } else {
        throw invalid_argument("Bad input expression");
    }
}

void step(Frame &pt) {
    pt.x += pt.dx;
    pt.y += pt.dy;
    if (pt.dx != 0)
        pt.dx = pt.dx - (pt.dx / abs(pt.dx));
    pt.dy -= 1;
}

bool inBounds(const Frame &pt, const Input &bounds) {
    return pt.x >= bounds.xmin && pt.x <= bounds.xmax && pt.y >= bounds.ymin && pt.y <= bounds.ymax;
}

// returns min int if inadmissible
// y if admissible, where y=max height achieved
int admissible(const Frame &pt, const Input &bounds) {
    Frame f = pt;
    int ymax = numeric_limits<int>::min();

    while (f.x <= bounds.xmax && f.y >= bounds.ymin) {
        step(f);
        ymax = max(ymax, f.y);
        if (inBounds(f, bounds)) 
            return ymax;
    }
    return numeric_limits<int>::min();
}

int partOne(const Input &bounds) {
    struct Frame frame;
    frame.x = 0;
    frame.y = 0;

    int ymax = 0;
    for (int dx = 1; dx < bounds.xmax; ++dx) {
        for (int dy = bounds.ymin; dy <= -bounds.ymin; ++dy) {
            frame.dx = dx;
            frame.dy = dy;
            int res = admissible(frame, bounds);
            ymax = max(res, ymax);
        }
    }

    return ymax;
}

int partTwo(const Input &bounds) {
    struct Frame frame;
    frame.x = 0;
    frame.y = 0;
    int count = 0;

    for (int dx = 1; dx <= bounds.xmax; ++dx) {
        for (int dy = bounds.ymin; dy <= -bounds.ymin; ++dy) {
            frame.dx = dx;
            frame.dy = dy;
            if (admissible(frame, bounds) != numeric_limits<int>::min()) {
                //cout << dx << ", " << dy << endl;
                ++count;
            }
        }
    }

    return count;
}

int main(int argc, char **argv) {
    struct Input input = fread(argv[1]);

    //cout << partOne(input);
    cout << partTwo(input);
}