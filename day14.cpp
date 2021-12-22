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
#include <limits>
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

template <typename T>
void pm(unordered_map<T, long> &m) {
    for (auto it = m.begin(); it != m.end(); ++it) {
        cout << (*it).first << ": " << (*it).second << endl;
    }
}

long partTwo(vector<char> &polymer, const Rules &rules, const size_t steps) {

    unordered_map<string, long> freqs;
    for (size_t i=0; i < polymer.size()-1; ++i) {
        string pair = string() + polymer[i] + polymer[i+1];
        if (!freqs.count(pair))
            freqs[pair] = 0;
        ++freqs[pair];
    }

    for (size_t step=0; step < steps; ++step) {
        unordered_map<string, long> newFreqs;

        for (auto it = freqs.begin(); it != freqs.end(); ++it) {
            const string &bigram = (*it).first;
            if (rules.count(bigram)) {
                string left = string(1, bigram[0]) + rules.at(bigram);
                string right = string(1, rules.at(bigram)) + bigram[1];

                if (!newFreqs.count(left))
                    newFreqs[left] = 0;
                if (!newFreqs.count(right))
                    newFreqs[right] = 0;
                newFreqs[left] += freqs[bigram];
                newFreqs[right] += freqs[bigram];
            }
        }

        freqs = newFreqs;
    }

    unordered_map<char, long> letterfreqs;
    for (auto it = freqs.begin(); it != freqs.end(); ++it) {
        char left = (*it).first[0];
        char right = (*it).first[1];

        if (!letterfreqs.count(left))
            letterfreqs[left] = 0;
        if (!letterfreqs.count(right))
            letterfreqs[right] = 0;
        letterfreqs[left] += it->second;
        letterfreqs[right] += it->second;
    } 

    // since everything else is duplicated
    ++letterfreqs[polymer[0]];
    ++letterfreqs[polymer[polymer.size()-1]];

    long greatestFreq = 0;
    long leastFreq = numeric_limits<long>::max();

    for (auto it = letterfreqs.begin(); it != letterfreqs.end(); ++it) {
        it->second /= 2;
        greatestFreq = max(greatestFreq, it->second);
        leastFreq = min(leastFreq, it->second);
    }

    pm(letterfreqs);
    //auto comparator = [](const pair<char, size_t> &p1, const pair<char, size_t> &p2) {
    //    return p1.second < p2.second;
    //};

    //int greatestFreq, leastFreq;
    //greatestFreq = max_element(letterfreqs.begin(), letterfreqs.end(), comparator)->second;
    //leastFreq = min_element(letterfreqs.begin(), letterfreqs.end(), comparator)->second;
    return greatestFreq - leastFreq;
}

vector<char> toPolymer(const string &line) {
    vector<char> cs;
    for (auto &c : line)
        cs.push_back(c);
    return cs;
}

int main(int argc, char **argv) {
    shared_ptr<Input> input = read(argv[1]);
    vector<char> polymer = toPolymer(input->init);
    //cout << partOne(polymer, input->rules, stoi(argv[2])) << endl;
    cout << partTwo(polymer, input->rules, stoi(argv[2])) << endl;
}