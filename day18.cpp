#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <unordered_map>
//#include <cmath>
using namespace std;

// 3*(3+4) + 2*(3*17 + 2*5) = 21 + 122 = 143
// a Node can either have a left & right,
// or a value,
// never both at the same time
enum ParentSide { left = 0, right = 1 };
struct Node {
    struct Node *left = nullptr, *right = nullptr, *parent = nullptr;
    ParentSide parentSide;
    int value = -1;
    int depth = 0;
};

// string to node[ptr]
Node* ston(const string &line) {
    deque<Node*> stk;
    int depth = 0;

    for (auto &c : line) {
        if (isdigit(c)) {
            Node *node = new Node;
            node->value = (c - '0');
            node->depth = depth;
            stk.push_back(node);
        }
        if (c == ']') {
            Node *node = new Node;
            node->depth = --depth;

            stk.back()->parent = node;
            stk.back()->parentSide = ParentSide::left;
            node->right = stk.back();
            stk.pop_back();

            stk.back()->parent = node;
            stk.back()->parentSide = ParentSide::right;
            node->left = stk.back();
            stk.pop_back();

            stk.push_back(node);
        }
        if (c == '[') {
            ++depth; 
        }
    }
    return stk.back();
}

deque<Node*> fread(const string &filename) {
    deque<Node*> buf;
    ifstream file;
    file.open(filename);
    string line;
    while (getline(file, line)) {
        buf.push_back(ston(line));
    }
    return buf;
}

Node *leftmost(Node *node) {
    Node *cur = node;
    while (cur && cur->left)
        cur = cur->left;
    return cur;
}

Node *rightmost(Node *node) {
    Node *cur = node;
    while (cur && cur->right)
        cur = cur->right;
    return cur;
}

Node *nearestLeftNeighbor(Node *node) {
    Node *cur = node;
    if (!cur || !cur->parent)
        return nullptr;

    ParentSide dir = cur->parentSide;
    //cout << "dir: " << dir << endl;
    cur = cur->parent; //->parent;
    while (cur) {
        if (dir != ParentSide::right) {
            Node *temp = rightmost(cur->left);
            if (temp && temp != cur)
                return temp;
        }
        dir = cur->parentSide;
        cur = cur->parent;
    }
    return nullptr;
}

Node *nearestRightNeighbor(Node *node) {
    Node *cur = node;
    if (!cur || !cur->parent)
        return nullptr;

    ParentSide dir = cur->parentSide;
    cur = cur->parent; //->parent;
    while (cur) {
        if (dir != ParentSide::left) {
            Node *temp = leftmost(cur->right);
            if (temp)
                return temp;
        }
        dir = cur->parentSide;
        cur = cur->parent;
    }
    return nullptr;
}

Node* canSplit(Node *node) {
    if (!node)
        return nullptr;
    if (node->value >= 10)
        return node;

    auto *left_res = canSplit(node->left);    
    auto *right_res = canSplit(node->right);    
    if (left_res)
        return left_res;
    if (right_res)
        return right_res;
    return nullptr;
}

void split(Node *node) {
    Node* toSplit = canSplit(node);
    if (!toSplit)
        return;

    toSplit->left = new Node;
    toSplit->left->value = toSplit->value / 2;
    toSplit->left->parent = toSplit;
    toSplit->left->parentSide = ParentSide::right;
    toSplit->left->depth = toSplit->depth + 1;

    toSplit->right = new Node;
    toSplit->right->value = (toSplit->value + 1) / 2;
    toSplit->right->parent = toSplit;
    toSplit->right->parentSide = ParentSide::left;
    toSplit->right->depth = toSplit->depth + 1;

    toSplit->value = -1;
}

Node* canExplode(Node *node) {
    if (!node)
        return nullptr;
    if (node->depth > 4)
        return node;

    auto *left_res = canExplode(node->left);
    auto *right_res = canExplode(node->right);
    if (left_res)
        return left_res;
    if (right_res)
        return right_res;
    return nullptr;
}

void explode(Node *node) {
    Node* toExplode = canExplode(node);
    //cout << "explode: " << toExplode->value << endl;
    Node* parent = toExplode->parent;

    if (!toExplode)
        return;

    Node* nearestLeft = nearestLeftNeighbor(parent);
    if (nearestLeft) {
        //cout << "left: " << nearestLeft->value << endl;
        nearestLeft->value += parent->left->value;
    }
    
    Node* nearestRight = nearestRightNeighbor(parent);
    if (nearestRight) {
        //cout << "right: " << nearestRight->value << endl;
        nearestRight->value += parent->right->value;
    }

    //parent->parent = toExplode->parent->parent;
    parent->left = nullptr;
    parent->right = nullptr;
    parent->value = 0;
    //delete toExplode;
}

// returns true if reducible
// false otherwise
bool reduce(Node *node) {
    if (canExplode(node)) {
        explode(node);
        return true;
    }
    if (canSplit(node)) {
        split(node);
        return true;
    }
    return false;
}

void traverse(Node *root) {
    if (!root)
        return;
    if (root->value != -1)// && root->parent)
        cout << " " << root->value << " "; //":" << root->depth << "," << root->parent->depth<< " ";
    else
        cout << "[";
    traverse(root->left);
    traverse(root->right);
    if (root->value == -1)
        cout << "]";
}

void incrDepths(Node *root) {
    if (!root)
        return;
    ++(root->depth);
    incrDepths(root->left);
    incrDepths(root->right);
}

Node *add(Node *left, Node *right) {
    Node *root = new Node;
    incrDepths(left);
    incrDepths(right);
    root->left = left;
    root->left->parent = root;
    root->left->parentSide = ParentSide::right;

    root->right = right;
    root->right->parent = root;
    root->right->parentSide= ParentSide::left;

    return root;
}

Node *add(deque<Node*> nodes) {
    while (nodes.size() >= 2) {
        //reduce(nodes[0]);
        //reduce(nodes[1]);
        Node *temp = add(nodes[0], nodes[1]);
        while (reduce(temp)) {
            //traverse(temp);
            //cout << endl;
        }
        nodes.pop_front(); // 0
        nodes.pop_front(); // 1
        nodes.push_front(temp);
    }

    while (reduce(nodes.front())) {}
    return nodes.front();
}

int magnitude(Node *node) {
    if (!node)
        return 0;
    if (node->value > -1)
        return node->value;
    return (3 * magnitude(node->left)) + (2 * magnitude(node->right));
}

// hacky and slow
int maxMagnitude(const string &filename) {
    ifstream file;
    file.open(filename);
    string line;
    vector<string> buf;
    while (getline(file, line))
        buf.push_back(line);

    int out = 0;
    for (int i = 0; i < buf.size(); ++i) {
        for (int j = 0; j < buf.size(); ++j) {
            if (i != j) {
                auto inode = ston(buf[i]);
                auto jnode = ston(buf[j]);
                auto sum = add(inode, jnode);
                while (reduce(sum)) {}
                out = max(magnitude(sum), out);
            }
        }
    }
    return out;
}

int main(int argc, char **argv) {
    auto buf = fread(argv[1]);
    
    // part one
    Node *res = add(buf);
    cout << endl << magnitude(res) << endl;

    // part two
    cout << endl << maxMagnitude(argv[1]) << endl;
}