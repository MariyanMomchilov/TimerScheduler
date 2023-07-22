#ifndef CB_QUEUE
#define CB_QUEUE

#include <vector>
#include <atomic>
#include <mutex>
#include "./common_types.h"

struct CallbackQueue {
    struct Pair {
        std::size_t id;
        callback_t cb;
    };

    std::atomic_flag closed{false};
    std::vector<Pair> callbacks;
    std::mutex mtx;

    bool pushCallback(std::size_t id, const callback_t &cb);
    bool removeCallback(std::size_t id);
};

#endif