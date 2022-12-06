#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <fstream>
#include <map>
#include <set>
#include <memory>
using namespace std;

typedef map<string, vector<string>> cavemap;
void pmap(const cavemap &m) {
    for (auto &kv : m) {
        cout << kv.first << "-";
        for (auto &x : kv.second)
            cout << x << ",";
        cout << endl;
    }
}

shared_ptr<cavemap> read(const string &filename) {
    ifstream file;
    file.open(filename);
    string line, from, to;
    cavemap m; // or define this as static, return by reference (e.g.)

    while (getline(file, line)) {
        const int delim_i = line.find('-');
        from = line.substr(0, delim_i);
        to = line.substr(delim_i+1, line.length());
        m[from].push_back(to); // TODO does this handle vector constructor?
        m[to].push_back(from);
    }
    return make_shared<cavemap>(m); // might not be necessary due to RVO?
}

// number of paths from start to end
// visiting each small cave at most once
int partOne(shared_ptr<cavemap> m) {

    struct State {
        string cave;
        set<string> visited;
    };

    int paths = 0;
    deque<State> q = {
        {"start", set<string> {"start"}}
    };
    while (q.size()) {
        State state = q.front(); // don't pass this by reference -- front reference gets invalidated by pop
        q.pop_front();

        if (state.cave == "end") {
            ++paths;
        } else {
            for (string candidate : m->at(state.cave)) {
                if (isupper(candidate[0]) ||
                    (islower(candidate[0]) && !state.visited.count(candidate))) {
                    set<string> new_visited = state.visited;
                    new_visited.insert(candidate);
                    q.push_back({candidate, new_visited});
                }
            }
        }
    }

    return paths;
}

int partTwo(shared_ptr<cavemap> m) {

    struct State {
        string cave;
        set<string> visited;
        bool smallcaveVisitedTwice;
    };

    int paths = 0;
    deque<State> q = {
        {"start", set<string> {"start"}, false}
    };
    while (q.size()) {
        State state = q.front();
        q.pop_front();

        if (state.cave == "end") {
            ++paths;
            continue;
        }
        for (string candidate : m->at(state.cave)) {
            if ((islower(candidate[0]) && !state.visited.count(candidate))
                || isupper(candidate[0])
            ) {
                set<string> new_visited = state.visited;
                new_visited.insert(candidate);
                q.push_back({candidate, new_visited, state.smallcaveVisitedTwice});
            } else if (islower(candidate[0]) && state.visited.count(candidate)
                && !state.smallcaveVisitedTwice && candidate != "start") {
                q.push_back({candidate, state.visited, true});
            }
        }
    }

    return paths;
}

int main(int argc, char **argv) {
    shared_ptr<cavemap> m = read(argv[1]);
    // pmap(*m); // is this bad?

    cout << partOne(m) << endl;
    cout << partTwo(m) << endl;
}