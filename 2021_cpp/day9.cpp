#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <utility>
#include <iterator>
#include <set>
#include <deque>

using namespace std;

vector<vector<int>> vread(const string &filename) {
    vector<vector<int>> grid;
    vector<int> row;
    ifstream file;
    file.open(filename);
    string line;

    while (getline(file, line)) {
        row.clear();
        for (auto &c : line)
            row.push_back(c - '0');
        grid.push_back(row);
    }

    return grid;
}

void pv(const vector<int> v) {
    for (const auto &x : v)
        cout << x << " ";
    cout << endl;
}

void pvv(const vector<vector<int>> vv) {
    for (const auto &v: vv)
        pv(v);
}

vector<pair<int, int>> neighbors(vector<vector<int>> &grid, int i, int j, int &m, int &n) {
    pair<int, int> adjs[4] = {
        make_pair(-1, 0),
        make_pair(1, 0),
        make_pair(0, -1),
        make_pair(0, 1),
    };

    vector<pair<int, int>> v;
    for (auto dxdy : adjs) {
        int _i = i + dxdy.first;
        int _j = j + dxdy.second;
        if ((_i >= 0) && (_i < m) && (_j >= 0) && (_j < n))
            v.push_back(make_pair(_i, _j));
    }
    return v;
}

bool lowest(vector<vector<int>> &grid, int i, int j, int &m, int &n) {
    for (auto xy : neighbors(grid, i, j, m, n)) {
        if (grid[i][j] >= grid[xy.first][xy.second])
            return false;
    }
    return true;
}

vector<pair<int, int>> lowestPoints(vector<vector<int>> &grid, int &m, int &n) {
    vector<pair<int, int>> points;
    for (int i=0; i < m; ++i) {
        for (int j=0; j < n; ++j) {
            if (lowest(grid, i, j, m, n))
                points.push_back(make_pair(i, j));
        }
    }

    return points;
}

int partOne(vector<vector<int>> &grid, int &m, int &n) {
    int sum = 0;
    for (auto &p : lowestPoints(grid, m, n))
        sum += 1 + grid[p.first][p.second];
    return sum;
}

// 1. find set S of all lowest points
// 2. bfs from that lowest point, remove any lowest point we come across if it's in S
// 3. result of that search is a basin
int partTwo(vector<vector<int>> &grid, int &m, int &n) {
    vector<pair<int, int>> lvec = lowestPoints(grid, m, n);
    set<pair<int, int>> lset(lvec.begin(), lvec.end());
    
    vector<vector<int>> basins;
    vector<int> basin;

    set<pair<int, int>> visited;
    deque<pair<int, int>> q = {lvec[0]};

    while (q.size() > 0 && lset.size() >= 0) {
        pair<int, int> xy = q.front();
        q.pop_front();
        if (!visited.count(xy)) // TODO more elegant
            basin.push_back(grid[xy.first][xy.second]);
            visited.insert(xy);

        if (lset.count(xy) > 0)
            lset.erase(xy);

        // discover more nodes in this basin
        vector<pair<int, int>> adj = neighbors(grid, xy.first, xy.second, m, n);
        for (auto &_xy : adj) {
            if (!visited.count(_xy) && grid[_xy.first][_xy.second] < 9)
                q.push_back(_xy);
        }

        // start a new basin
        if (lset.size() > 0 && q.size() == 0) {
            basins.push_back(basin);
            basin.clear();
            q.push_back(*lset.begin());
            // lset.erase(*lset.begin());
        }
    }

    basins.push_back(basin);
    int product = 1;
    vector<int> sizes;
    for (auto &basin : basins)
        sizes.push_back(basin.size());
    sort(sizes.begin(), sizes.end(), greater<int>());
    for (int i=0; i<min(3, static_cast<int>(sizes.size())); ++i)
        product *= sizes[i];
    return product;
}

int main(int argc, char **argv) {
    vector<vector<int>> grid = vread(argv[1]);
    int m = grid.size();
    int n = grid[0].size();
    // cout << m << ", " << n << endl;

    cout << partOne(grid, m, n) << endl;
    cout << partTwo(grid, m, n) << endl;
}