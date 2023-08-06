#include "./log_timer_scheduler.h"


LoggerTimerScheduler::LoggerTimerScheduler(std::size_t threadCount = 1): TimerScheduler(threadCount), logLimit(1000), logs(logLimit) {}
LoggerTimerScheduler::LoggerTimerScheduler(std::size_t threadCount = 1, std::size_t logLimit = 1000): TimerScheduler(threadCount), logLimit(logLimit), logs(logLimit) {}

TimerScheduler::TimerHandle LoggerTimerScheduler::scheduleSingle(ns_t timeout, callback_t &&callback) {
    std::size_t id = idSequenceNumber.fetch_add(1, std::memory_order_relaxed);
    time_point_t created = std::chrono::system_clock::now();
    logs[id] = Log{created};
    container.push(TimerInfo{created, timeout, callback, 0, id});
    return TimerScheduler::TimerHandle{id};
}

TimerScheduler::TimerHandle LoggerTimerScheduler::scheduleRepeat(ns_t interval, callback_t &&callback, int repeatCount = -1) {
    std::size_t id = idSequenceNumber.fetch_add(1, std::memory_order_relaxed);
    time_point_t created = std::chrono::system_clock::now();
    logs[id] = Log{created};
    container.push(TimerInfo{created, interval, callback, repeatCount, id});
    return TimerHandle{id};
}

void LoggerTimerScheduler::processReadyToExecuteTimerInfo(TimerInfo &info) {
    if (info.callCount > 0 || info.callCount == -1) {
        if (info.callCount > 0) {
            --info.callCount;
        }
        info.started = std::chrono::system_clock::now();
        logs[info.id].scheduledForExecution.push_back(info.started);
        container.push(info);
    }
}

void LoggerTimerScheduler::processNotReadyToExecuteTimerInfo(TimerInfo &info) {
    info.started = std::chrono::system_clock::now();
    container.push(info);
}
