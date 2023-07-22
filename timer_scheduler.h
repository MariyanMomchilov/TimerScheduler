#ifndef TIME_SCHED
#define TIME_SCHED

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "./timer_info_container.h"
#include "./callback_queue.h"


struct TimerScheduler {
    struct TimerHandle {
        std::size_t id;
    };

    TimerScheduler(std::size_t threadCount);
    TimerHandle scheduleSingle(ns_t timeout, callback_t &&callback);
    TimerHandle scheduleRepeat(ns_t interval, callback_t &&callback);
    TimerHandle scheduleRepeat(ns_t interval, int repeatCount, callback_t &&callback);
    bool cancelTimer(TimerHandle &handle);
    void wait();

private:
    void listen();

    TimerInfoContainer container;
    std::vector<std::thread> workerThreads;
    std::thread timersReadyListener;
    std::condition_variable callbackReadyToProcessEvent;
    std::atomic<std::size_t> idSequenceNumber;
    CallbackQueue queue;
};

#endif
