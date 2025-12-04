#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
using namespace std;

#define ONE_MASK 0x1

/*
 v=4][t=2][1][ 1 packet  ][v=1][t=2]]1][ 1 packet ][v=5][t=2][0][length = 13    ][v=6][t=4][  120   ]
[100][010][1][00000000001][001][010][1]00000000001][101][010][0][000000000001011][110][100][01111000]
[           a                                                                                       ]
                          [                                     b                                   ]
                                                   [                        c                       ]
                                                                                 [         d        ]

a: op
 \
  b: op
   \
    c: op
     \
      d: literal

outermost: always a single operator or literal, never >1 operator
*/

/*
 v=3             2sub   v=0               22bits   v=0 lit  10?      5      
[011|000|1|00000000010][000|000|0|000000000010110][000|100|01010|1][011|000|1|01100100010]0000000010000100011000111000110100

101000000000000101101100100010000000000101100010000000010111110000110110100001101011000110001010001111010100011110000000
*/

unsigned htoi(char h) {
    if (h >= '0' && h <= '9')
        return h - '0';
    if (h >= 'A' && h <= 'F')
        return h - 'A' + 10;
    throw invalid_argument("Invalid input h");
}

typedef vector<bool> Bits;
Bits stoh(const string &line) {
    Bits bits;
    Bits buf(4);
    
    for (auto &c : line) {
        unsigned n = htoi(c);
        buf.clear();
        for (int i = 0; i < 4; ++i)
            buf.push_back((n >> i) & ONE_MASK);
        for (auto it = buf.rbegin(); it != buf.rend(); ++it)
            bits.push_back(*it);
    }
    return bits;
}

Bits fread(const string &filename) {
    string line;
    ifstream file;
    file.open(filename);
    getline(file, line);
    return stoh(line);
}

struct Packet {
    unsigned version;
    unsigned id;
    size_t size;

    long value = 0;
    int mode = 0;
    vector<Packet> packets; 
};

// mutates i, like a file ptr
unsigned read(const Bits &bits, size_t &i, size_t n) {
    unsigned out = 0;
    for (int j = 0; j < n; ++j) {
        out <<= 1;
        out |= bits[i++];
    }
    return out;
}

size_t nextGreatestFourMultiple(size_t n) {
    if (n % 4 == 0)
        return n;
    return ((n / 4) + 1) * 4;
}

vector<Packet> parseRange(const Bits &bits, size_t &i, size_t end);
vector<Packet> parseN(const Bits &bits, size_t &i, unsigned npackets);

Packet parseLiteral(const Bits &bits, size_t &i) {
    struct Packet literal;
    literal.version = read(bits, i, 3);
    literal.id = read(bits, i, 3);

    bool hasNextGroup = read(bits, i, 1);
    long value = 0;
    size_t nGroups = 0;
    while (hasNextGroup) {
        ++nGroups;
        value <<= 4;
        value |= read(bits, i, 4);
        hasNextGroup = read(bits, i, 1);
    }
    ++nGroups;
    value <<= 4;
    value |= read(bits, i, 4);

    literal.size = nextGreatestFourMultiple(3 + 3 + (5 * nGroups));
    literal.value = value;
    return literal;
}

Packet parseOp(const Bits &bits, size_t &i) {
    struct Packet op;
    op.version = read(bits, i, 3);
    op.id = read(bits, i, 3);
    op.mode = read(bits, i, 1);
    op.size = 3 + 3 + 1;

    if (op.mode == 0) { // n bits
        unsigned nBits = read(bits, i, 15);
        op.size += 15;
        op.packets = parseRange(bits, i, i + nBits);
        op.size += nBits;
    } else { // n subpackets
        unsigned nSubpackets = read(bits, i, 11);
        op.size += 11;
        op.packets = parseN(bits, i, nSubpackets);
        for (auto &p : op.packets)
            op.size += p.size;
    }
    return op;
}

vector<Packet> parseN(const Bits &bits, size_t &i, unsigned npackets) {
    unsigned n = 0;
    vector<Packet> packets;

    while (n < npackets) {
        unsigned version = read(bits, i, 3);
        unsigned id = read(bits, i, 3);
        struct Packet packet;
        i -= 6;
        if (id == 4) {
            packet = parseLiteral(bits, i);
        } else {
            packet = parseOp(bits, i);
        }

        packets.push_back(packet);
        ++n;
    }
    return packets;
}

vector<Packet> parseRange(const Bits &bits, size_t &i, size_t end) {
    vector<Packet> packets;
    
    while (i < end) {
        unsigned version = read(bits, i, 3);
        unsigned id = read(bits, i, 3);
        struct Packet packet;
        i -= 6;
        if (id == 4) {
            packet = parseLiteral(bits, i);
        } else {
            packet = parseOp(bits, i);
        }

        packets.push_back(packet);
    }
    
    return packets;
}

unsigned sumVersions(Packet &p, int indent=0) {
    struct Packet node = p;
    unsigned sum = 0; 
    for (int i=0; i < indent; ++i)
        cout << "    ";
    cout << node.version << "(" << node.packets.size() << ")" << endl;

    sum += node.version;
    if (node.packets.size()) {
        for (auto &sub : node.packets) {
            sum += sumVersions(sub, indent + 1);
        }
    }

    return sum;
}

// alternatively, a topsort
long eval(Packet &p, int indent=0) {
    struct Packet node = p;
    long res = 0;

    for (int i=0; i < indent; ++i)
        cout << "  ";

    switch (node.id) {
        case 0: // sum
            cout << "+" << endl;
            for (auto packet : node.packets)
                res += eval(packet, indent + 1);
            break;
        case 1: // product
            cout << "*" << endl;
            res = 1;
            for (auto packet : node.packets)
                res *= eval(packet, indent + 1);
            break;
        case 2: // minimum
            cout << "min" << endl;
            res = numeric_limits<int>::max();
            for (auto packet : node.packets)
                res = min(res, eval(packet, indent + 1));
            break;
        case 3: // maximum
            cout << "max" << endl;
            res = 0;
            for (auto packet : node.packets)
                res = max(res, eval(packet, indent + 1));
            break;
        case 4: // literal
            cout << node.value << endl;
            res += node.value;
            break;
        case 5: // gt
            cout << ">" << endl;
            res = eval(node.packets[0]) > eval(node.packets[1], indent + 1);
            break;
        case 6: // lt
            cout << "<" << endl;
            res = eval(node.packets[0]) < eval(node.packets[1], indent + 1);
            break;
        case 7: // eq
            cout << "==" << endl;
            res = eval(node.packets[0]) == eval(node.packets[1], indent + 1);
            break;
    }

    return res;
}

void printBits(const Bits &bits) {
    for (auto b : bits)
        cout << b;
    cout << endl;
}

int main(int argc, char **argv) {
    Bits bits;
    if (argc == 2)
        bits = fread(argv[1]);
    else
        bits = stoh(argv[2]);

    size_t i = 0;
    Packet op = parseOp(bits, i);

    // part one
    //cout << sumVersions(op) << endl;

    // part two
    cout << eval(op) << endl;
}
