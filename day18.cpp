#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <unordered_map>
using namespace std;

// 3*(3+4) + 2*(3*17 + 2*5) = 21 + 122 = 143

// a Node can either have a left & right,
// or a value,
// never both at the same time
struct Node {
    struct Node *left = nullptr, *right = nullptr, *parent = nullptr;
    int value = NULL;
    int depth = 0;
};

unordered_map<size_t, size_t> findMatchingBrackets(const string &line) {
    unordered_map<size_t, size_t> brackets; // brackets[i] = index of closing bracket matching s[i]
    deque<size_t> stk;
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] == '[') 
            stk.push_back(i);
        if (line[i] == ']') {
            brackets[stk.back()] = i;
            stk.pop_back();
        }
    }
    return brackets;
}

//void insert(Node *root, size_t depth, bool left) {
//    
//}

// [[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]
// string to node
// for every pair [...],
//
// map (id -> Node)
// map (id -> parent id) where parent id is ?
// or
// s[i] == [ => new node, whose 
//  - left is 
//      - s[i+1] is [ => ston(s[i+1:matching closing bracket index])
//      - s[i+1] is digit => NULL, value.left = atoi(s[i+1...])
//  - right is
//      - s[next char after matching closing bracket index & comma if exists] is [ => ston(s[next char...its matching closing bracket index])
//      - s[next char...] is digit => NULL, value.right = atoi(s[next char...])
Node* ston(const string &line, unordered_map<size_t, size_t> brackets) {
    Node *root = new Node;

    size_t ptr = 1;
    int digit;
    while (ptr < line.size()) {
        digit = 0;
        if (line[ptr] == '[') {
            string left_str = line.substr(ptr, brackets[ptr] - ptr + 1);
            cout << "LS: " << left_str << endl;
            root->left = ston(left_str, brackets);
            ptr = brackets[ptr];
            if (brackets[ptr] + 2 < line.size()) {
                size_t right_start = brackets[ptr] + 2; // 1 for comma, one for [
                root->right = ston(line.substr(right_start, brackets[right_start] - right_start + 1), brackets);
                ptr = brackets[right_start];
            }
        } else {
            while (isdigit(line[ptr]) && ptr < line.size()) {
                digit *= 10;
                digit += (line[ptr] - '0');
                ++ptr;
            }
            if (root->values.first) {
                root->values.second = digit;
            } else {
                root->values.first = digit;
            }
        }
        ++ptr;
    }

    return root;
}

void fread(const string &filename, vector<Node*> buf) {
    ifstream file;
    file.open(filename);
    string line;
    while (getline(file, line)) {
        auto brackets = findMatchingBrackets(line);
        buf.push_back(ston(line, brackets));
    }
}

void reduce(Node *node) {

}

void traverse(Node *root) {
    if (!root)
        return;
    traverse(root->left);
    cout << (root->values.first) << "," << (root->values.second) << endl;
    traverse(root->right);
}

int main(int argc, char **argv) {
    string line = argv[1];
    auto brackets = findMatchingBrackets(line);
    auto *node = ston(line, brackets);
    traverse(node);
}