#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <deque>
#include <set>
using namespace std;

vector<vector<int>> read(const string &filename) {
    ifstream file;
    file.open(filename);
    vector<vector<int>> grid;
    string line;

    while (getline(file, line)) {
        vector<int> row;
        for (char &c : line)
            row.push_back(c - '0');
        grid.push_back(row); 
    }
    return grid;
}

struct Point {
    int x;
    int y;
};
inline bool operator<(const Point &lhs, const Point &rhs) {
    return lhs.x < rhs.x;
}

template <typename T>
void pvv(vector<vector<T>> vv) {
    for (auto &v: vv) {
        for (auto &x : v)
            cout << x;
        cout << endl;
    }
}

vector<Point> adjs(vector<vector<int>> &grid, int &x, int &y) {
    const int m = grid.size();
    const int n = grid[0].size();
    vector<Point> v;

    for (int _x = x-1; _x <= x+1; ++_x) {
        for (int _y = y-1; _y <= y+1; ++_y) {
            if (_x == x && _y == y)
                continue;
            if (_x >= 0 && _x < m && _y >= 0 && _y < n) {
                // cout << x << "," << y << ": " << _x << "," << _y << endl;
                v.push_back({_x, _y});
            }
        }
    }
    return v;
}

// TODO unused function?
int bfsIncr(vector<vector<int>> &grid, int &i, int &j) {
    deque<Point> q;
    q.push_back({i, j});
    set<Point> visited;

    while (q.size()) {
        Point pt = q.front();
        q.pop_front();
        ++grid[pt.x][pt.y];
        visited.insert(pt);

        for (auto &_pt : adjs(grid, pt.x, pt.y)) {
            if (visited.count(_pt) == 0 && grid[_pt.x][_pt.y] < 9)
                q.push_back(_pt);
        }
    }
}

// returns # total flashes in grid this step
// modifies grid inplace
int step(vector<vector<int>> &grid) {
    int flashes = 0;

    // pvv(grid);
    // cout << endl;

    // first, energy level of each octopus increases by 1
    deque<Point> temp;
    for (int i=0; i < grid.size(); ++i) {
        for (int j=0; j < grid[0].size(); ++j) {
            ++grid[i][j];
            if (grid[i][j] > 9)
                temp.push_back({i, j});
        }
    }

    // any octopus with energy level greater than 9 flashes
    // if this causes an octopus to have an energy level greater than 9, 
    // it also flashes. 
    // this process continues as long as new octopuses keep having their
    // energy level increased beyond 9.
    // (an octopus can only flash at most once per step.)
    /*
    for (int i=0; i < grid.size(); ++i) {
        for (int j=0; j < grid[0].size(); ++j) {
            deque<Point> q = temp;

            set<Point> visited;
            while (q.size()) {
                Point pt = q.front();
                q.pop_front();
                visited.insert(pt);
                
                if (grid[pt.x][pt.y] > 9) {
                    ++flashes;
                    grid[pt.x][pt.y] = -1;
                }

                for (auto adj : adjs(grid, pt.x, pt.y)) {
                    if (!visited.count(adj) && grid[adj.x][adj.y] != -1)
                        q.push_back(adj);
                }
            }
        }
    }
    */

    bool updated = true;
    while (updated) {
        updated = false;
        for (int i=0; i < grid.size(); ++i) {
            for (int j=0; j < grid[0].size(); ++j) {
                if (grid[i][j] > 9) {
                    grid[i][j] = -1;
                    updated = true;
                    for (auto &adj : adjs(grid, i, j)) {
                        if (grid[adj.x][adj.y] != -1)
                            ++grid[adj.x][adj.y];
                    }
                }
            }
        }
    }

    for (int i=0; i < grid.size(); ++i) {
        for (int j=0; j < grid[0].size(); ++j) {
            if (grid[i][j] == -1) {
                ++flashes;
                grid[i][j] = 0;
            }
        }
    }

    return flashes;
}

bool isSynchronized(vector<vector<int>> &grid) {
    for (int i=0; i < grid.size(); ++i) {
        for (int j=0; j < grid[0].size(); ++j) {
            if (grid[i][j] != 0)
                return false;
        }
    }
    return true;
}

int partOne(vector<vector<int>> &grid, int epochs) {
    int flashes = 0;
    for (int epoch=0; epoch < epochs; ++epoch) {
        flashes += step(grid); 
    }
    return flashes;
}

int partTwo(vector<vector<int>> &grid, int epochs) {
    for (int epoch=0; epoch < epochs; ++epoch) {
        if (isSynchronized(grid)) {
            return epoch;
        }
        step(grid);
    }
    return -1;
}

int main(int argc, char **argv) {
    vector<vector<int>> grid = read(argv[1]);
    cout << partOne(grid, stoi(argv[2])) << endl;

    grid = read(argv[1]);
    cout << partTwo(grid, stoi(argv[2])) << endl;
}