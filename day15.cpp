#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <queue>
#include <unordered_map>
#include <limits>
#include <utility>
#include <algorithm>
using namespace std;

typedef vector<vector<unsigned>> Grid;
shared_ptr<Grid> fread(const string &filename) {
    ifstream file;
    file.open(filename);
    string line;
    shared_ptr<Grid> grid = make_shared<Grid>();
    vector<unsigned> row;

    while (getline(file, line)) {
        row.clear();
        for (auto &c : line)
            row.push_back(c - '0');
        grid->push_back(row);
    }

    return grid;
}

/**
 * part one
 * 
 * dist = a map from coord -> dist to that coord
 * where for an m x n grid, coord(x, y) = xn + y
 */
//unsigned coord(unsigned x, unsigned y, unsigned ncols) {
//    return (x * ncols) + y;
//}

typedef pair<unsigned, unsigned> Point;
vector<Point> adjs(Grid &grid, size_t m, size_t n, unsigned x, unsigned y) {
    vector<Point> out;
    for (int i = x - 1; i <= x + 1; ++i) {
        for (int j = y - 1; y <= y + 1; ++j) {
            if (i == x && j == y)
                continue;
            if (i >= 0 && i < m && j >= 0 && j < n)
                out.push_back(make_pair(i, j));
        }
    }
    return out;
}

// hash function so unordered_map can use Points/pairs as a key
struct point_hash {
    size_t operator ()(const Point &pt) const {
        auto x = hash<unsigned>{}(pt.first);
        auto y = hash<unsigned>{}(pt.second);
        return x ^ y;
    }
};

struct ptr_comparator {
    public:
    bool operator()(pair<shared_ptr<unsigned>, Point> &a, pair<shared_ptr<unsigned>, Point> &b) {
        return *(a.first) < *(b.first);
    }
};

unsigned dijkstra(Grid &grid) {
    size_t m = grid.size();
    size_t n = grid[0].size();
    priority_queue<
        pair<shared_ptr<unsigned>, Point>,
        vector<pair<shared_ptr<unsigned>, Point>>, //greater<pair<shared_ptr<unsigned>, Point>>
        ptr_comparator
    > pq;
    unordered_map<Point, shared_ptr<unsigned>, point_hash> dist;

    // initial conditions
    for (unsigned i = 0; i < m; ++i) {
        for (unsigned j = 0; j < n; ++j) {
            Point pt = {i, j};
            if (i == 0 && j == 0) {
                dist[pt] = make_shared<unsigned>(0);
                pq.push(make_pair(dist[pt], pt));
            } else {
                dist[pt] = make_shared<unsigned>(numeric_limits<unsigned>::max());
                pq.push(make_pair(dist[pt], pt));
            }
        }
    }

    while (!pq.empty()) {
        pair<shared_ptr<unsigned>, Point> cost_coord = pq.top();
        pq.pop();

        unsigned x = cost_coord.second.first;
        unsigned y = cost_coord.second.second;
        for (auto &adj : adjs(grid, m, n, x, y)) {
            cout << pq.size() << endl;
            unsigned cost = *(dist[adj]) + grid[x][y];
            *(dist[adj]) = min(cost, *(dist[adj]));
        }
    }

    return *(dist[make_pair(m-1, n-1)]);
}

void printGrid(Grid &grid) {
    for (auto &v : grid) {
        for (auto &e : v)
            cout << e;
        cout << endl;
    }
}

int main(int argc, char **argv) {
    shared_ptr<Grid> grid = fread(argv[1]);
    printGrid(*grid);
    cout << dijkstra(*grid) << endl;
}