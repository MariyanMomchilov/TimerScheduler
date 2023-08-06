#include <atomic>
#include <chrono>
#include <cassert>
#include "../common_types.h"
#include "../timer_scheduler.h"
#include <iostream>

void notExecutedBeforeScheduleTime(int threadsCount = 1) {
    TimerScheduler scheduler(threadsCount);
    std::atomic_int counter(0);

    int timersNumber = 20;
    time_point_t times[20]; 
    for (int i = 0; i < timersNumber; i++) {
        times[i] = std::chrono::system_clock::now();

        scheduler.scheduleSingle(ns_t(0), [i, times, &counter](){
            assert(times[i] <= std::chrono::system_clock::now());
            counter += 1;
        });
    }

    while (counter.load() < 20);
    scheduler.close();
    scheduler.wait();
}

int main() {
    notExecutedBeforeScheduleTime(2);
    return 0;
}