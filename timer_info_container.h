#ifndef TIMER_INFO
#define TIMER_INFO

#include <mutex>
#include <queue>
#include <vector>
#include <condition_variable>
#include <atomic>
#include "./common_types.h"


struct TimerInfo {
    time_point_t started;
    ns_t timeout;
    callback_t cb;
    int callCount;
    std::size_t id;

    bool operator>(const TimerInfo& rhs) const {
        return started + timeout > rhs.started + rhs.timeout;
    }
};


struct TimerInfoContainer {
    void push(const TimerInfo &info);
    bool remove(std::size_t id);
    std::vector<TimerInfo> popReady();
    size_t length();
    void waitOnEmpty();
    void close();

private:
    std::mutex mtx;
    std::priority_queue<TimerInfo, std::vector<TimerInfo>, std::greater<TimerInfo>> queue;
    std::condition_variable containerNotEmptyEvent;
    std::atomic<bool> closed;
};

#endif