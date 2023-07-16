#include <iostream>
#include "../timer_info_container.h"


int main() {
    TimerInfoContainer container;

    TimerInfo t1{std::chrono::system_clock::now(), ns_t(3000000000), []() {
        std::cout << "Hello from timer 1\n";
    }};
    container.push(t1);

    TimerInfo t2{std::chrono::system_clock::now() + ns_t(500000000), ns_t(1000000000), []() {
        std::cout << "Hello from timer 2\n";
    }};
    container.push(t2);

    while (container.length() > 0)
    {
        auto ready = container.popReady();
        for (const TimerInfo& r: ready) {
            r.cb();
        }
    }
    return 0;
}