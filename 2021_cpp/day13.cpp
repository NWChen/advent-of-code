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

typedef vector<vector<bool>> Grid;

struct Fold {
    string dim; 
    int value;
};

struct Input {
    shared_ptr<Grid> grid;
    vector<Fold> folds;
};

Point readPt(const string &line) {
    int commaIndex = line.find(',');
    string x = line.substr(0, commaIndex);
    string y = line.substr(commaIndex+1, line.length());

    return {
        stoi(x),
        stoi(y)
    };
}

shared_ptr<Input> read(const string &filename) {
    // vector<Point> v;
    ifstream file;
    file.open(filename);
    string line;

    struct Input input;
    int xmax = 0, ymax = 0;

    // find max x / y
    while (getline(file, line) && line.size()) {
        Point point = readPt(line);
        xmax = max(xmax, point.x);
        ymax = max(ymax, point.y);
    }

    Grid grid(
        ++ymax,
        vector<bool>(++xmax, false)
    );
    vector<Fold> f;
    file.clear();
    file.seekg(0);

    while (getline(file, line)) {
        if (line.size() && line[0] != 'f') {
            Point pt = readPt(line);
            grid[pt.y][pt.x] = true;
        } else {
            int eqIndex = line.find('=');
            if (eqIndex != string::npos) {
                string dim = line.substr(eqIndex-1, 1); // note the second param is length of substr, NOT end index!
                string value = line.substr(eqIndex+1, line.length());
                f.push_back({dim, stoi(value)});
            }
        }
    }

    input.grid = make_shared<Grid>(grid);
    input.folds = f;
    return make_shared<Input>(input);
}

void foldOverY(Grid &grid, int &y) {
    int last_unfolded = y - 1;
    for (Grid::iterator it = grid.begin() + y + 1; it < grid.end() && last_unfolded >= 0; ++it) {
        for (int i=0; i < (*it).size(); ++i) {
            bool f = (*it).at(i);
            grid[last_unfolded][i] = grid[last_unfolded][i] | f;
        }
        --last_unfolded; 
    }

    // now truncate the rest of the grid
    for (int i = grid.size(); i > y ; --i) {
        grid.pop_back();        
    }
}

void foldOverX(Grid &grid, int &x) {
    for (int offset = 1; offset < grid[0].size() && (x - offset) >= 0; ++offset) {
        for (int y=0; y < grid.size(); ++y) {
            grid[y][x - offset] = grid[y][x-offset] | grid[y][x+offset];
        }
    }

    // now truncate the rest of the grid
    int xmax = grid[0].size();
    for (int y = 0; y < grid.size(); ++y) {
        for (int i = xmax - 1; i >= x; --i) {
            grid[y].pop_back();    
        }
    }
}

void pgrid(Grid &grid) {
    for (auto &row : grid) {
        for (auto e : row) {
            if (e)
                cout << "#";
            else
                cout << ".";
        }
        cout << endl;
    }
}

int partOne(shared_ptr<Grid> grid, Fold fold) {
    if (fold.dim == "x")
        foldOverX(*grid, fold.value); // TODO this pass-by-reference of object of shared_ptr is strange, and maybe doesn't do what you think?
    if (fold.dim == "y")
        foldOverY(*grid, fold.value);

    // int ymax = (*(input->grid)).size(), xmax = (*(input->grid)).at(0).size();
    int out = 0;
    for (auto &row : *grid) {
        for (auto e : row) {
            out += e;
        }
    }
    return out;
}

int main(int argc, char **argv) {
    shared_ptr<Input> input = read(argv[1]);

    cout << partOne(input->grid, input->folds.at(0)) << endl;
    // pgrid(*(input->grid));
    // for (auto &fold : input->folds) {
        // cout << fold.dim << "=" << fold.value << endl;
    // }
}