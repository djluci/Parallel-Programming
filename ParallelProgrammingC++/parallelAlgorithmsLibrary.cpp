#include <vector>
#include <algorithm>
#include <execution>
#include <iostream>

int main() {
    std::vector<int> v = {1, 3, 5, 7, 9};
    std::for_each(std::execution::par, v.begin(), v.end(), [](int &n) { n *= 2; });
    for (int n : v) std::cout << n << " "; // Output: 2 6 10 14 18
    return 0;
}