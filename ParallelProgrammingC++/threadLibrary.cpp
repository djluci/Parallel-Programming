#include <iostream>
#include <thread>

void print_message() {
    std::cout << "Hello from a thread" << std::endl;
}

int main() {
    std::thread t(print_message);
    t.join(); // makes sure thread is completed before exiting program
    return 0

}