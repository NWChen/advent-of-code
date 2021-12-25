#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
using namespace std;

#define ONE_MASK 0x1   // 0001
#define THREE_MASK 0x7 // 0111
#define FOUR_MASK 0xF  // 1111
#define ELEVEN_MASK 0x7FF
#define FIFTEEN_MASK 0x7FFF
#define GROUP_LENGTH 5

unsigned htoi(char h) {
    if (h >= '0' && h <= '9')
        return h - '0';
    if (h >= 'A' && h <= 'F')
        return h - 'A' + 10;
    throw invalid_argument("Invalid input h");
}

typedef vector<bool> Bits;
Bits fread(const string &filename) {
    string line;
    ifstream file;
    file.open(filename);
    getline(file, line);
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

struct Packet {
    unsigned version;
    unsigned id;
    size_t size;
};

struct Literal : Packet {
    unsigned value;
};

struct Operator : Packet {
    unsigned mode; // length type ID
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

Literal parseLiteral(const Bits &bits, size_t &i) {
    struct Literal literal;
    literal.version = read(bits, i, 3);
    literal.id = read(bits, i, 3);

    bool hasNextGroup = read(bits, i, 1);
    unsigned value = 0;
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

Operator parseOp(const Bits &bits, size_t &i) {
    struct Operator op;
    op.version = read(bits, i, 3);
    op.id = read(bits, i, 3);
    op.mode = read(bits, i, 1);

    if (op.mode == 0) { // n bits
        unsigned nBits = read(bits, i, 15);
    } else { // n subpackets
        unsigned nSubpackets = read(bits, i, 11);
    }
}

// ---

// returns (literal value, number of groups read)
pair<unsigned, size_t> readLiteralGroups(const Bits &bits, size_t &i) {
    bool hasNextGroup = read(bits, i, 1);
    unsigned value = 0;
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
    return make_pair(value, nGroups);
}

//vector<Packet> parseNBits(const Bits &bits, size_t &i, size_t &nBits) {
//    
//}
//
//vector<Packet> parseNSubpackets(const Bits &bits, size_t &i, size_t &nSubpackets) {
//
//}

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
vector<Packet> parse(const Bits &bits, size_t &i,
    int nBits = -1, int nBitsParsed = 0,         // for parsing operands with total length = nBits
    int nPackets = -1, int nPacketsParsed = 0) { // for parsing operands with total #packets = nPackets

    vector<Packet> packets;
    if ((bits.size() < 6) || (nBitsParsed == nBits) || (nPacketsParsed == nPackets))
        return packets;

    // bits 1-3 are version: ...[111]011100... => 7
    unsigned version = read(bits, i, 3);

    // bits 3-6 are type ID: ...111[011]100... => 3
    unsigned typeId = read(bits, i, 3);

    if (typeId == 4) {
        struct Literal literal;
        literal.version = version;
        literal.id = typeId;

        auto pr = readLiteralGroups(bits, i);
        literal.value = pr.first;
        size_t consumedSoFar = 3 + 3 + (5 * pr.second);
        i = nextGreatestFourMultiple(consumedSoFar);
        packets.push_back(literal);
    } else {
        struct Operator op;
        op.version = version;
        op.id = typeId;
        op.mode = read(bits, i, 1);

        if (op.mode == 0) {
            unsigned nBitsInSubpackets = read(bits, i, 15);
            op.packets = parse(bits, i, nBitsInSubpackets, nBitsParsed + 3 + 3 + 1 + 15, -1, 0);
            //op.packets = parseNBits(bits, i, nBitsInSubpackets);
        } else {
            unsigned nSubpackets = read(bits, i, 11);
            op.packets = parse(bits, i, -1, 0, nSubpackets, nPacketsParsed + 1);
            //op.packets = parseNSubpackets(bits, i, nSubpackets);
        }
        packets.push_back(op);
        for (auto packet : parse(bits, i))
            packets.push_back(packet);
    }

    return packets;
}

void printBits(const Bits &bits) {
    for (auto b : bits)
        cout << b;
    cout << endl;
}

int main(int argc, char **argv) {
    Bits bits = fread(argv[1]);
    //decodeLiteral(bits, 24);
    printBits(bits);
    size_t i = 0;
    auto p = parse(bits, i);
}
