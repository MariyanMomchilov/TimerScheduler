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
    bool cancelTimer(TimerHandle &handle);
    void wait();
    bool checkIfCurrentlyExecuting(std::size_t id);
    void close();
    bool isClosed();

    TimerHandle scheduleSingle(ns_t timeout, callback_t &&callback);
    TimerHandle scheduleRepeat(ns_t interval, callback_t &&callback, int repeatCount = -1);
private:
    std::atomic_size_t idSequenceNumber;
    TimerInfoContainer container;
    CallbackQueue queue;

    void processReadyToExecuteTimerInfo(TimerInfo &info);
    void processNotReadyToExecuteTimerInfo(TimerInfo &info);

    void listen();
    std::atomic<bool> closed;
    std::vector<std::atomic_size_t> callbackIds;
    std::vector<std::thread> workerThreads;
    std::thread timersReadyListener;
    std::condition_variable callbackReadyToProcessEvent;
};

#endif
