#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <utility>
#include <algorithm>
#include <math.h>
using namespace std;

typedef vector<vector<int>> Grid;
shared_ptr<Grid> fread(const string &filename) {
    ifstream file;
    file.open(filename);
    string line;
    shared_ptr<Grid> grid = make_shared<Grid>();
    vector<int> row;

    while (getline(file, line)) {
        row.clear();
        for (auto &c : line)
            row.push_back(c - '0');
        grid->push_back(row);
    }

    return grid;
}

typedef pair<int, int> Point;
vector<Point> adjs(Grid &grid, int m, int n, int x, int y) {
    vector<Point> out;
    if (x - 1 >= 0)
        out.push_back(make_pair(x - 1, y));
    if (x + 1 < m)
        out.push_back(make_pair(x + 1, y));
    if (y - 1 >= 0)
        out.push_back(make_pair(x, y - 1));
    if (y + 1 < n)
        out.push_back(make_pair(x, y + 1));
    return out;
}

// hash function so unordered_map can use Points/pairs as a key
struct point_hash {
    size_t operator ()(const Point &pt) const {
        auto x = hash<int>{}(pt.first);
        auto y = hash<int>{}(pt.second);
        return x ^ y;
        //return (x << 8) & y;
    }
};

struct ptr_comparator {
    public:
    bool operator()(pair<int, Point> &a, pair<int, Point> &b) {
        return a.first > b.first;
    }
};

int heur(int x, int y, int i, int j) {
    return abs(x - i) + abs(y - j); // manhattan distance
    //return sqrt((x*x - i*i) + (y*y - j*j));
}

int dijkstra(Grid &grid) {
    int m = grid.size();
    int n = grid[0].size();

    unordered_map<Point, int, point_hash> dist;
    unordered_set<Point, point_hash> visited;

    Point origin = make_pair(0, 0);
    dist[origin] = 0;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (!(i == 0 && j == 0)) {
                Point pt = make_pair(i, j);
                dist[pt] = numeric_limits<int>::max();
            }
        }
    }
    priority_queue<pair<int, Point>, vector<pair<int, Point>>, ptr_comparator> pq;

    pq.push(make_pair(0, origin));
    visited.insert(origin);
    while (pq.size()) {
        auto pr = pq.top();
        int current_distance = pr.first;
        Point point = pr.second; // this point is the node with the lowest cost to visit, so far
        visited.insert(point);
        if (point.first == m-1 && point.second == n-1)
            return dist[make_pair(point.first, point.second)];
        pq.pop();

        //if (current_distance > dist[point])
        //    continue;

        for (auto adj : adjs(grid, m, n, point.first, point.second)) {
            //int cost = current_distance + grid[adj.first][adj.second];
            int cost = dist[make_pair(point.first, point.second)] + grid[adj.first][adj.second];
            int heur_cost = cost + heur(m-1, n-1, adj.first, adj.second); //abs(m-1 - adj.first) + abs(n-1 - adj.second);
            if (cost < dist[adj] && visited.count(adj) == 0) {
                dist[adj] = cost;
                pq.push(make_pair(heur_cost, adj)); // for any given adj/neighbor, that neighbor enters pq at most once; and the lowest cost of getting to adj gets popped first
            }
        }
    }

    return dist[make_pair(m-1, n-1)];
}

// just copied from wikipedia pseudocode
// basically the same exact thing as above 
int astar(Grid &grid) {
    int m = grid.size();
    int n = grid[0].size();

    priority_queue<pair<int, Point>, vector<pair<int, Point>>, ptr_comparator> open;
    unordered_map<Point, Point, point_hash> from;
    unordered_map<Point, int, point_hash> gscore, fscore;
    unordered_set<Point, point_hash> visited;

    Point origin = make_pair(0, 0);
    gscore[origin] = 0;
    fscore[origin] = heur(m-1, n-1, 0, 0);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (!(i == 0 && j == 0)) {
                Point pt = make_pair(i, j);
                gscore[pt] = numeric_limits<int>::max();
                fscore[pt] = numeric_limits<int>::max();
            }
        }
    }
    open.push(make_pair(fscore[origin], origin));

    while (!open.empty()) {
        auto current = open.top();
        auto cost = current.first;
        auto pt = current.second;
        visited.insert(pt);
        open.pop();

        if (pt.first == m-1 && pt.second == n-1)
            return gscore[pt];

        for (auto adj : adjs(grid, m, n, pt.first, pt.second)) {
            int g = gscore[pt] + grid[adj.first][adj.second];
            if (g < gscore[adj]) {
                gscore[adj] = g;
                fscore[adj] = g + heur(m-1, n-1, adj.first, adj.second);
                if (visited.count(adj) == 0)
                    open.push(make_pair(fscore[adj], adj));
            }
        }
    }

    return 0;
}

shared_ptr<Grid> expandByFive(Grid &grid) {
    size_t m = grid.size();
    size_t n = grid[0].size();
    size_t row, col;

    shared_ptr<Grid> big = make_shared<Grid>();
    big->resize(5 * m);
    for (row = 0; row < big->size(); ++row)
        big->at(row).resize(5 * n);

    // first batch row
    for (col = 0; col < 5 * n; ++col) {
        for (row = 0; row < m; ++row) {
            if (col < n) {
                big->at(row).at(col) = grid[row][col];
            } else {
                int incr = big->at(row)[col-n] + 1;
                if (incr > 9)
                    incr = 1;
                big->at(row).at(col) = incr;
            }            
        }
    }

    // remaining rows 
    for (row = m; row < 5 * m; ++row) {
        for (col = 0; col < 5 * n; ++col) {
            if (row >= m || col >= n) {
                int incr = big->at(row - m)[col] + 1;
                if (incr > 9)
                    incr = 1;
                big->at(row).at(col) = incr;
            }
        }
    }
    
    return big;
}

void printGrid(Grid &grid) {
    for (auto &v : grid) {
        for (auto &e : v)
            cout << e;
        cout << endl;
    }
}

int main(int argc, char **argv) {
    argv[1] = "input/input15.txt";
    shared_ptr<Grid> grid = fread(argv[1]);
    //printGrid(*expandByFive(*grid));

    // part one
    //cout << dijkstra(*grid) << endl;
    //cout << astar(*grid) << endl;

    // part two
    cout << dijkstra(*expandByFive(*grid)) << endl;
}