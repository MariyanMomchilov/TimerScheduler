#include "./timer_scheduler.h"
#include <iostream>

int fib(int n) {
    if (n <= 2) {
        return 1;
    }

    return fib(n - 2) + fib(n - 1);
}

std::function<void()> calcFib(int n) {
    return [n]() {
        std::cout << "n = " << n << " is " << fib(n) << '\n';
    };
}

int main() {

    TimerScheduler sched(3);
    std::vector<TimerScheduler::TimerHandle> handlers;

    for (int i = 0; i < 1000; i++) {
        sched.scheduleRepeat(ns_t(1000000000), calcFib((i + 1) % 40), 10);
    }

    sched.scheduleSingle(ns_t(11000000000), [&sched]() {
        sched.close();
    });

    sched.wait();
    return 0;
}