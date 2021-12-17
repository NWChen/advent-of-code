#include <fstream>
#include <iostream>
#include <unordered_map>
#include <list>
#include <string>
#include <memory>
#include <deque>
#include <utility>
#include <vector>
#include <algorithm>
using namespace std;

typedef unordered_map<string, char> Rules;
struct Input {
    Rules rules; // pair insertion rules
    string init; // polymer template
};

pair<string, char> parse(const string &line) {
    size_t arrowIndex = line.find(" -> ");
    return make_pair(
        line.substr(0, arrowIndex),
        line.substr(arrowIndex + 4, line.size())[0]
    );
}

shared_ptr<Input> read(const string &filename) {
    ifstream file;
    file.open(filename, ifstream::in);
    string line;

    auto input = make_shared<Input>();
    getline(file, line);
    input->init = line;
    getline(file, line);

    while (getline(file, line)) {
        auto p = parse(line);
        input->rules[p.first] = p.second;
    }
    return input;
}

void pp(vector<char> &p) {
    for (auto &c : p)
        cout << c << " ";
    cout << endl;
}

// O(n) additional memory :/
// TODO we don't actually have to create a new one. we just need freqs?
void step(vector<char> &polymer, const Rules &rules) {
    vector<char> out;
    out.push_back(polymer[0]);

    for (int i=0; i < polymer.size() - 1; ++i) {
        char p1 = polymer[i];
        char p2 = polymer[i+1];
        string lookup = string() + p1 + p2;
        if (rules.count(lookup))
            out.push_back(rules.at(lookup));
        out.push_back(p2);
    }

    polymer = out;
}

int partOne(vector<char> &polymer, const Rules &rules, const size_t steps) {
    for (size_t i=0; i < steps; ++i)
        step(polymer, rules);

    unordered_map<char, size_t> freqs;
    for (auto &c : polymer) {
        if (freqs.count(c))
            ++freqs.at(c);
        else
            freqs[c] = 0;
    }

    vector<size_t> vals;
    for (auto &kv : freqs)
        vals.push_back(kv.second);
    sort(vals.begin(), vals.end());
    return vals.back() - vals.front();
}

void oldstep(list<char> &polymer, const Rules &rules) {
    deque<pair<char, int>> insertions;
    list<char>::const_iterator it = polymer.begin();
    char prev = *it;
    size_t i = 0;

    for (++it; it != polymer.end(); ++it) {
        string pr = string() + prev + *it;

        if (rules.count(pr)) {
            auto insertion = make_pair(rules.at(pr), ++i);
            insertions.push_back(insertion);
        }
        prev = *it;
    }

    for (auto &pr : insertions)
        cout << pr.first << ":" << pr.second << " ";
    cout << endl;

    int nInserts = 0;
    int targetIndex = 0;
    //list<char>::const_iterator it = polymer.begin();
    auto itp = polymer.begin();
    int itIndex = 0;

    while (insertions.size()) {

        pair<char, int> insertion = insertions.front();
        targetIndex = insertion.second + nInserts;
        cout << targetIndex << ": " << insertion.second << endl;
        while (itIndex < targetIndex) {
            ++itIndex;
            ++itp;
        }
        polymer.insert(itp, insertion.first);
        ++nInserts;
        insertions.pop_front();
    }
}

//list<char> toPolymer(const string &line) {
vector<char> toPolymer(const string &line) {
    vector<char> cs;
    for (auto &c : line)
        cs.push_back(c);
    return cs;
}

int main(int argc, char **argv) {
    shared_ptr<Input> input = read(argv[1]);
    vector<char> polymer = toPolymer(input->init);
    cout << partOne(polymer, input->rules, stoi(argv[2])) << endl;
}