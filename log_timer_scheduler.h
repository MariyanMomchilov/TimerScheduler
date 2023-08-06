#pragma once
#include "./timer_scheduler.h"
#include "./common_types.h"
#include "./log.h"

struct LoggerTimerScheduler: TimerScheduler {
    std::size_t logLimit;
    std::vector<Log> logs;

    // keep array of logs
    // add constructor with log limit number
    LoggerTimerScheduler(std::size_t threadCount = 1);
    LoggerTimerScheduler(std::size_t threadCount = 1, std::size_t logLimit = 1000);
    virtual TimerHandle scheduleSingle(ns_t timeout, callback_t &&callback) override;
    virtual TimerHandle scheduleRepeat(ns_t interval, callback_t &&callback, int repeatCount = -1) override;
protected:
    virtual void processReadyToExecuteTimerInfo(TimerInfo &info) override;
    virtual void processNotReadyToExecuteTimerInfo(TimerInfo &info) override;
};