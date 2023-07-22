#ifndef TIMER_INFO
#define TIMER_INFO

#include <mutex>
#include <queue>
#include <vector>
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


class TimerInfoContainer {
public:
    TimerInfoContainer() = default;
    ~TimerInfoContainer() = default;
    TimerInfoContainer(const TimerInfoContainer&) = delete;
    TimerInfoContainer& operator=(const TimerInfoContainer&) = delete;

    void push(const TimerInfo &info);
    bool remove(std::size_t id);
    std::vector<TimerInfo> popReady();
    size_t length();

private:
    std::mutex mtx;
    std::priority_queue<TimerInfo, std::vector<TimerInfo>, std::greater<TimerInfo>> queue;
};

#endif