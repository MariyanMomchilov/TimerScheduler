#include <thread>
#include <iterator>
#include <mutex>
#include <atomic>
#include <functional>
#include "./callback_queue.h"
#include "./timer_scheduler.h"
#include "./process_callback_worker.h"
#include <iostream>

TimerScheduler::TimerHandle TimerScheduler::scheduleSingle(ns_t timeout, callback_t &&callback) {
    std::size_t id = idSequenceNumber.fetch_add(1, std::memory_order_relaxed);
    container.push(TimerInfo{std::chrono::system_clock::now(), timeout, callback, 0, id});
    return TimerScheduler::TimerHandle{id};
}

TimerScheduler::TimerHandle TimerScheduler::scheduleRepeat(ns_t interval, callback_t &&callback, int repeatCount) {
    std::size_t id = idSequenceNumber.fetch_add(1, std::memory_order_relaxed);
    container.push(TimerInfo{std::chrono::system_clock::now(), interval, callback, repeatCount, id});
    return TimerHandle{id};
}

TimerScheduler::TimerScheduler(std::size_t threadCount): workerThreads(threadCount), callbackIds(threadCount), idSequenceNumber(1), closed(false) {
    listen();

    for (int i = 0; i < threadCount; i++) {
        callbackIds[i].store(0, std::memory_order_relaxed);
        workerThreads[i] = std::thread([this, i](){
            processCallbackWorker(queue, callbackReadyToProcessEvent, callbackIds[i]);
        });
    }
}

void TimerScheduler::wait() {
    for (int i = 0; i < workerThreads.size(); i++) {
        workerThreads[i].join();
    }
    timersReadyListener.join();
}

void TimerScheduler::listen() {

    timersReadyListener = std::thread([this]() {
        while (!closed.load(std::memory_order_relaxed)) {
            container.waitOnEmpty();
            std::vector<TimerInfo> readyTimers = container.popReady();
            bool someReady = false;
            for (TimerInfo& info: readyTimers) {
                if (!checkIfCurrentlyExecuting(info.id) && queue.pushCallback(info.id, info.cb)) {
                    someReady = true;
                    processReadyToExecuteTimerInfo(info);
                } else {
                    processNotReadyToExecuteTimerInfo(info);
                }
            }
            if (someReady) {
                callbackReadyToProcessEvent.notify_all();
            }
        }
        callbackReadyToProcessEvent.notify_all();
    });
}

void TimerScheduler::close() {
    closed.store(true, std::memory_order_relaxed);
    queue.closed.store(true, std::memory_order_release);
    container.close();
}

bool TimerScheduler::isClosed() {
    return closed.load(std::memory_order_relaxed);
}

bool TimerScheduler::checkIfCurrentlyExecuting(std::size_t id) {
    for (int i = 0; i < callbackIds.size(); i++) {
        if (callbackIds[i].load(std::memory_order_relaxed) == id) {
            return true;
        }
    }
    return false;
}

bool TimerScheduler::cancelTimer(TimerScheduler::TimerHandle &handle) {
    container.remove(handle.id);
    queue.removeCallback(handle.id);
    for (int i = 0; i < callbackIds.size(); i++) {
        if (callbackIds[i].load(std::memory_order_relaxed) == handle.id) {
            return false;
        }
    }
    return true;
}

void TimerScheduler::processReadyToExecuteTimerInfo(TimerInfo &info) {
    if (info.callCount > 0 || info.callCount == -1) {
        if (info.callCount > 0) {
            --info.callCount;
        }

        info.started = std::chrono::system_clock::now();
        container.push(info);
    }
}

void TimerScheduler::processNotReadyToExecuteTimerInfo(TimerInfo &info) {
    info.started = std::chrono::system_clock::now();
    container.push(info);
}