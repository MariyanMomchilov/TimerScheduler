#include "./timer_info_container.h"

void TimerInfoContainer::push(const TimerInfo &info) {
    mtx.lock();
    queue.push(info);
    mtx.unlock();
}

std::vector<TimerInfo> TimerInfoContainer::popReady() {
    std::vector<TimerInfo> ready;
    mtx.lock();
    auto now = std::chrono::system_clock::now();
    auto top = queue.top();
    while (queue.size() > 0 && top.started + top.timeout <= now)
    {
        ready.push_back(top);
        queue.pop();
        top = queue.top();
    }
    mtx.unlock();
    return ready;
}

size_t TimerInfoContainer::length() {
    mtx.lock();
    size_t len = queue.size();
    mtx.unlock();
    return len;
}
