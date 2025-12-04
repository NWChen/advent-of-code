#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <assert.h>
#include <map>
#include <utility>
using namespace std;

//const int BOARD_DIM = 5;
const int MARKER = -1;

struct Input {
    vector<int> numbers;
    vector<vector<vector<int>>> boards;
};

vector<int> csvStrToNums(const string line, const char delimiter=' ') {
    stringstream ss(line);
    string substr;
    vector<int> buf;
    while (ss.good()) {
        getline(ss, substr, delimiter);
        if (substr.size())
            buf.push_back(stoi(substr));
    }
    return buf;
}

Input vread(const string filename) {
    ifstream file;
    string line;
    string substr;
    file.open(filename);

    struct Input input;
    vector<int> numbers;
    vector<int> row;
    vector<vector<int>> board;
    vector<vector<vector<int>>> boards;

    // header
    getline(file, line);
    input.numbers = csvStrToNums(line, ',');

    while (getline(file, line)) {
        // finished reading this board, prepare to read the next one
        if (line.size() == 0 && board.size()) {
            boards.push_back(board);
            board.clear();
        }
        if (line.size()) {
            row = csvStrToNums(line, ' ');
            board.push_back(row);
        }
    }
    boards.push_back(board);
    input.boards = boards;
    return input;
}

map<int, pair<int, int>> numToRowCol(vector<vector<int>>& board) {
    map<int, pair<int, int>> m;
    for (int row=0; row < board.size(); ++row) {
        for (int col=0; col < board[row].size(); ++col) {
            m[board[row][col]] = make_pair(row, col);
        }
    }
    return m;
}

bool isBingo(vector<vector<int>>& board, int r, int c) {
    const int BOARD_DIM = board.size();
    vector<int> bingoVec(BOARD_DIM), column;
    fill(bingoVec.begin(), bingoVec.end(), MARKER);

    for (auto row : board) {
        column.push_back(row[c]);
    }
    return board[r] == bingoVec || column == bingoVec;
}

int score(vector<vector<int>>& board) {
    int out = 0;
    for (auto row : board) {
        for (int num : row) {
            if (num != MARKER) {
                out += num;
            }
        }
    }
    return out;
}

int partOne(struct Input& input) {
  vector<map<int, pair<int, int>>> numToCoords;
    for (auto board : input.boards) {
        numToCoords.push_back(numToRowCol(board));
    }

    for (int num : input.numbers) {
        for (int i=0; i < input.boards.size(); ++i) {
            vector<vector<int>>& board = input.boards[i];
            if (numToCoords[i].count(num)) {
                pair<int, int> rowCol = numToCoords[i][num];
                board[rowCol.first][rowCol.second] = MARKER;
                if (isBingo(board, rowCol.first, rowCol.second)) {
                    return (num * score(board));
                }
            }
        }
    } 
    return -1;
}

int partTwo(struct Input& input) {
    vector<map<int, pair<int, int>>> numToCoords;
    vector<vector<vector<int>>> boards = input.boards;
    vector<int> &numbers = input.numbers;

    // TODO this is fucking nasty
    vector<map<int, pair<int, int>>> tempNumToCoords;
    vector<vector<vector<int>>> tempBoards;
    for (auto board : boards) {
        numToCoords.push_back(numToRowCol(board));
    }

    for (int num : numbers) {
        for (int i=0; i < boards.size(); ++i) {
            vector<vector<int>>& board = boards[i];

            // assumes num is in each board
            if (numToCoords[i].count(num)) {
                pair<int, int> rowCol = numToCoords[i][num];
                board[rowCol.first][rowCol.second] = MARKER;
                if (isBingo(board, rowCol.first, rowCol.second)) {
                    if (boards.size()==1) {
                        return (num * score(board));
                    }
                    continue;
                }
            }
            tempBoards.push_back(board);
            tempNumToCoords.push_back(numToCoords[i]);
        }
        boards = tempBoards;
        numToCoords = tempNumToCoords;
        tempBoards.clear();
        tempNumToCoords.clear();
    } 
    return -1;
}

int main(int argc, char **argv) {

    struct Input input = vread("input4.txt");
    int partOneAns = partOne(input);
    cout << partOneAns << endl;

    input = vread("input4.txt");
    int partTwoAns = partTwo(input);
    cout << partTwoAns << endl;
    // numToCoords
  
    // vector<int> expected;
    // expected = {7, 4, 9};
    // assert(csvStrToNums("7,4,9", ',') == expected);
    // expected = {22, 13, 17, 11, 0};
    // assert(csvStrToNums("22 13 17 11  0") == expected);
}