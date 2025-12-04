#include <iostream>
#include <algorithm>
#include <assert.h>

template<typename T>
class NVector {

public:

    NVector() {
        buf = (T*)malloc(sizeof(T) * capacity);
        size = 0;
    }

    ~NVector() {
        free(buf);
    }

    void push_back(const T &t) {
        if (size < capacity) {
            *(buf + size) = t;
            ++size;
        } else {
            capacity *= 2;
            size_t _alloc = sizeof(T*) * capacity; // exponential resizing
            buf = (T*)realloc(buf, _alloc);

            if (buf) {
                *(buf + size) = t;
                ++size;
            }
        }
    }

    T &operator[](size_t i) {
        return *(buf + i);
    }

    size_t length() {
        return size;
    }

    void print() {
        for (size_t i=0; i < size; ++i)
            std::cout << *(buf + i) << " ";
        std::cout << std::endl;
    }

    void sort() {
        qs(0, this->length()-1);
    }

    void clear() {
        capacity = 10;
        size = 0;
        buf = (T*)malloc(sizeof(T) * capacity);
    }

private:
    std::size_t capacity = 10;
    std::size_t size;
    T* buf = nullptr;

    // elements at start...(i-1) inclusive are < pivot
    // elements at i...end inclusive are >= pivot
    size_t partition(size_t start, size_t end) {
        size_t pivot = start;
        T pivotE = *(buf + start);

        for (size_t j = start+1; j <= end; ++j) {
            if (*(buf + j) <= pivotE) {
                ++pivot;
                std::swap(*(buf + pivot), *(buf + j));
            }
        }

        std::swap(*(buf + pivot), *(buf + start));
        return pivot;
    }

    void qs(size_t start, size_t end) {
        if (start >= end || start < 0)
            return;

        size_t pivot = partition(start, end);
        qs(start, pivot);
        qs(pivot+1, end);
    }
};

int main() {
    NVector<int> v;
    assert(v.length() == 0);
    v.push_back(1);
    assert(v.length() == 1);
    assert(v[0] == 1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    assert(v.length() == 4);
    assert(v[1] == 2);
    assert(v[2] == 3);
    assert(v[3] == 4);
    v.print();

    v.push_back(5);
    v.push_back(8);
    v.push_back(7);
    v.push_back(6);
    v.print();
    v.sort();
    v.print();
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
    assert(v[3] == 4);
    assert(v[4] == 5);
    assert(v[5] == 6);
    assert(v[6] == 7);
    assert(v[7] == 8);

    v.clear();
    assert(v.length() == 0);

    v.push_back(5);
    v.push_back(2);
    v.push_back(4);
    v.push_back(3);
    v.push_back(1);
    v.sort();
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
    assert(v[3] == 4);
    assert(v[4] == 5);
}