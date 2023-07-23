#include "./timer_info_container.h"


void TimerInfoContainer::push(const TimerInfo &info) {
    if (closed.load(std::memory_order_relaxed)) {
        return;
    }

    std::lock_guard<std::mutex> lock(mtx);
    queue.push(info);
    if (queue.size() == 1) {
        containerNotEmptyEvent.notify_one();
    }
}

std::vector<TimerInfo> TimerInfoContainer::popReady() {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<TimerInfo> ready;
    auto now = std::chrono::system_clock::now();
    while (queue.size() > 0)
    {
        auto top = queue.top();
        if (top.started + top.timeout <= now) {
            ready.push_back(top);
            queue.pop();
        } else {
            return ready;
        }
    }
    return ready;
}

size_t TimerInfoContainer::length() {
    std::lock_guard<std::mutex> lock(mtx);
    return queue.size();
}

void TimerInfoContainer::waitOnEmpty() {
    std::unique_lock<std::mutex> lock(mtx);
    containerNotEmptyEvent.wait(lock, [this](){
        return closed.load(std::memory_order_relaxed) || queue.size() > 0;
    });
}

void TimerInfoContainer::close() {
    closed.store(true, std::memory_order_relaxed);
}

bool TimerInfoContainer::remove(std::size_t id) {
    std::lock_guard<std::mutex> lock(mtx);
    bool removed = false;

    if (queue.size() == 0) {
        return false;
    }

    std::vector<TimerInfo> tmp(queue.size());
    while (queue.size() > 0 && queue.top().id != id) {
        tmp.push_back(queue.top());
        queue.pop();
    }
    if (queue.size() > 0) {
        queue.pop();
        removed = true;
    }

    for (int i = queue.size() - 1; i >= 0; --i) {
        queue.push(tmp[i]);
    }

    return removed;
}
