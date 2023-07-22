#include <thread>
#include <iterator>
#include <mutex>
#include <atomic>
#include <functional>
#include "./timer_scheduler.h"


void processCallbackWorker(CallbackQueue &queue, std::condition_variable &event) {
    std::unique_lock<std::mutex> lock(queue.mtx);
    while(!queue.closed.test(std::memory_order_relaxed)) {
        if (!lock.owns_lock()) {
            lock.lock();
        }

        event.wait(lock, [&queue]() -> bool {
            return !queue.closed.test(std::memory_order_relaxed) && queue.callbacks.size() > 0;
        });

        if (queue.callbacks.size() > 0) {
            std::function<void()> cb = queue.callbacks[0].cb;
            queue.callbacks.erase(queue.callbacks.begin());
            lock.unlock();
            cb();
        } else {
            lock.unlock();
        }
    }
}

TimerScheduler::TimerHandle TimerScheduler::scheduleSingle(ns_t timeout, callback_t &&callback) {
    std::size_t id = idSequenceNumber.fetch_add(1, std::memory_order_relaxed);
    container.push(TimerInfo{std::chrono::system_clock::now(), timeout, callback, 0, id});
    return TimerScheduler::TimerHandle{id};
}

TimerScheduler::TimerHandle TimerScheduler::scheduleRepeat(ns_t interval, callback_t &&callback) {
    std::size_t id = idSequenceNumber.fetch_add(1, std::memory_order_relaxed);
    container.push(TimerInfo{std::chrono::system_clock::now(), interval, callback, -1, id});
    return TimerScheduler::TimerHandle{id};
}

TimerScheduler::TimerHandle TimerScheduler::scheduleRepeat(ns_t interval, int repeatCount, callback_t &&callback) {
    std::size_t id = idSequenceNumber.fetch_add(1, std::memory_order_relaxed);
    container.push(TimerInfo{std::chrono::system_clock::now(), interval, callback, repeatCount, id});
    return TimerScheduler::TimerHandle{id};
}

TimerScheduler::TimerScheduler(std::size_t threadCount): workerThreads(threadCount), idSequenceNumber(0) {
    listen();

    for (int i = 0; i < threadCount; i++) {
        workerThreads[i] = std::thread([this](){
            processCallbackWorker(queue, callbackReadyToProcessEvent);
        });
    }
}

void TimerScheduler::wait() {
    timersReadyListener.join();
    for (int i = 0; i < workerThreads.size(); i++) {
        workerThreads[i].join();
    }
}

void TimerScheduler::listen() {
    timersReadyListener = std::thread([this]() {
        while (true) {
            std::vector<TimerInfo> ready = container.popReady();
            for (TimerInfo& info: ready) {
                if (queue.pushCallback(info.id, info.cb)) {
                    callbackReadyToProcessEvent.notify_one();
                    if (info.callCount > 0 || info.callCount == -1) {
                        if (info.callCount > 0) {
                            --info.callCount;
                        }
                        info.started = std::chrono::system_clock::now();
                        container.push(info);
                    }
                } else {
                    container.push(info);
                }
            }
        }
    });
}

bool TimerScheduler::cancelTimer(TimerScheduler::TimerHandle &handle) {
    // if (container.remove(handle.id) || queue.removeCallback(handle.id)) {
    //     return true;
    // };
    return false;
}
