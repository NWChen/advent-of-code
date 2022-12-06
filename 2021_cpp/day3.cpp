#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
using namespace std;

vector<int> vread(string filename) {
    ifstream file;
    vector<int> buf;
    string line;
    file.open(filename);

    while (getline(file, line)) {
        buf.push_back(stoi(line, nullptr, 2));
    }
    return buf;
}

// most ties with 1 priority
// least ties with 0 priority
int commonBit(int index, vector<int>& vec, bool mostCommon) {
    int ones = 0, zeros = 0;
    for (auto num : vec) {
        if (num & (1 << index)) {
            ++ones;
        } else {
            ++zeros;
        }
    }

    if (mostCommon)
        return (ones >= zeros);
    return 1 - (ones >= zeros);
}

int main(int argc, char **argv) {
    const int BITSTRING_LENGTH = 12;
    const string INPUT_FILE = argv[1];
    vector<int> buf = vread(INPUT_FILE);

    int ones[BITSTRING_LENGTH] = {0};
    int zeros[BITSTRING_LENGTH] = {0};
    for (auto num : buf) {
        for (int i=(BITSTRING_LENGTH-1); i >= 0; --i) {
            ones[i] += num & 1;
            zeros[i] += 1 - (num & 1);
            num >>= 1;
        }
    }

    // part 1
    int gamma = 0, epsilon = 0;
    for (int i=0; i < BITSTRING_LENGTH; ++i) {
        gamma <<= 1;
        epsilon <<= 1;
        gamma ^= (ones[i] > zeros[i]);
        epsilon ^= 1 - (ones[i] > zeros[i]);
    }

    cout << gamma << ", " << epsilon << endl;

    // part 2
    int oxygen, co2;
    vector<int> candidates = buf;
    vector<int> temp;

    // oxygen
    for (int i=(BITSTRING_LENGTH-1); i >= 0 && candidates.size() > 1; --i) {
        temp.clear();
        int mostCommonIthBit = commonBit(i, candidates, true);
        for (int candidate : candidates) {
            if ((candidate & (1 << i)) == (mostCommonIthBit << i)) {
                temp.push_back(candidate);
            }
        }
        candidates = temp;
    }
    oxygen = candidates[0];

    candidates = buf;
    for (int i=(BITSTRING_LENGTH-1); i >= 0 && candidates.size() > 1; --i) {
        temp.clear();
        int leastCommonIthBit = commonBit(i, candidates, false);
        for (int candidate : candidates) {
            if ((candidate & (1 << i)) == (leastCommonIthBit << i)) {
                temp.push_back(candidate);
            }
        }
        candidates = temp;
    }
    co2 = candidates[0];

    cout << oxygen << ", " << co2 << ", " << oxygen * co2 << endl;

    return 0;
}