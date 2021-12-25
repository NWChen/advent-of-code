#include "utils.h"
#include <iostream>
#include <vector>

template <typename T>
void pv(const std::vector<T> &v) {
    for (const auto &x : v)
        std::cout << x;
    std::cout << std::endl;
}