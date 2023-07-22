#include "timer_scheduler.h"
#include <iostream>

void thanks() {
    std::cout << "Thank you\n";
}

void gracias() {
    std::cout << "Gracias\n";
}

int main() {
    TimerScheduler sched(2);
    TimerScheduler::TimerHandle h1 = sched.scheduleRepeat(ns_t(2000000000), thanks);
    TimerScheduler::TimerHandle h2 = sched.scheduleRepeat(ns_t(4000000000), gracias);

    TimerScheduler::TimerHandle h3 = sched.scheduleSingle(ns_t(10000000000), [&sched]() {
        sched.scheduleSingle(ns_t(1000000000), []() { std::cout << "Thanks 2"; });
    });

    sched.wait();
    return 0;
}