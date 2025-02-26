#include <tbb/parallel_for.h>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v(1000, 1);
    tbb::parallel_for(size_t(0), v.size(), [&](size_t i) { v[i] += 1; });
    std::cout << v[0] << std::endl; // Output: 2
    return 0;
}