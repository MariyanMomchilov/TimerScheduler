#ifndef TIMER_INFO
#define TIMER_INFO

#include <mutex>
#include <queue>
#include <vector>
#include <functional>
#include <chrono>


using callback_t = std::function<void()>;

using ns_t = std::chrono::nanoseconds;
using time_point_t = std::chrono::time_point<std::chrono::system_clock, ns_t>;


struct TimerInfo {
    time_point_t started;
    ns_t timeout;
    callback_t cb;

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
    std::vector<TimerInfo> popReady();
    size_t length();

private:
    std::mutex mtx;
    std::priority_queue<TimerInfo, std::vector<TimerInfo>, std::greater<TimerInfo>> queue;
};

#endif