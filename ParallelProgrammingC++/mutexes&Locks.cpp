#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;

void safe_print(const std::string &message) {
    std::lock_guard<std::mutex> lock<mtx> // acquires lock for scope
    std::cout << message << std::endl;
}

int main() {
    std::thread t1(safe_print, "Thread 1");
    std::thread t2(safe_print, "thread 2");
    t1.join();
    t2.join();
    return 0;
}