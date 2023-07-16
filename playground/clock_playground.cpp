#include <iostream>
#include <chrono>

using ns_t = std::chrono::nanoseconds;

int main() {
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> start = std::chrono::system_clock::now();
    int a = 0;
    for (int i = 0; i < 10000; i++) {
        a += i;
    }
    std::cout << a << '\n';
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> end = std::chrono::system_clock::now();

    std::cout << (end - start).count() << '\n';
    return 0;
}