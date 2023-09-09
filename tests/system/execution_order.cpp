#include <atomic>
#include <chrono>
#include <cassert>
#include "../../common_types.h"
#include "../../timer_scheduler.h"
#include <iostream>

void executionOrderIsSequential(int threadsCount = 1) {
    TimerScheduler scheduler(threadsCount);
    std::atomic_int counter(0);

    int timersNumber = 20;
    time_point_t times[20];

    for (int i = 0; i < timersNumber; i++) {
        scheduler.scheduleSingle(ns_t(i + 1), [i, times, &counter](){
            counter += 1;
        });
    }

    while (counter.load() < 20);
    scheduler.close();
    scheduler.wait();
    for (int i = 0; i < 20; i++) {
        assert(times[i] <= times[i+1]);
    }
}

int main() {
    executionOrderIsSequential(2);
    return 0;
}
