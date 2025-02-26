#include <iostream>
#include <future>

int compute() {
    return 42;
}

int main() {
    std::future<int> result = std::async(compute); // Launches a thread
    std::cout << "Result: " << result.get() << std::endl; // Waits for the result
    return 0;
}