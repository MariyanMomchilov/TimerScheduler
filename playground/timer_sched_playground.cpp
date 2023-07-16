#include <chrono>
#include <iostream>
#include <thread>

using ns_t = std::chrono::nanoseconds;
using time_point_t = std::chrono::time_point<std::chrono::system_clock, ns_t>;

bool check_time(time_point_t start, ns_t interval) {
    return std::chrono::system_clock::now() >= start + interval;
}

void callback() {
    std::cout << "Hello from callback!\n";
}

void schedule_timers() {
    time_point_t start = std::chrono::system_clock::now();
    int repeat = 4;
    ns_t interval = ns_t(2000000000); // 2 seconds

    while (repeat > 0)
    {
        if (check_time(start, interval)) {
            callback();
            start = std::chrono::system_clock::now();
            --repeat;
        }
    }
}

int main() {
    std::thread t1 = std::thread(schedule_timers);
    std::cout << t1.get_id() << '\n';
    t1.join();
    std::cout << t1.get_id() << '\n';
    return 0;
}
