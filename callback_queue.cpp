#include <thread>
#include <iterator>
#include <vector>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include "./callback_queue.h"


bool CallbackQueue::pushCallback(std::size_t id, const callback_t &cb) {
    if (closed.load(std::memory_order_relaxed)) {
        return false;
    }

    std::lock_guard<std::mutex> lock(mtx);
    for (int i = 0; i < callbacks.size(); i++) {
        if (id == callbacks[i].id) {
            return false;
        }
    }
    callbacks.push_back(Pair{id, cb});
    return true;
}

void CallbackQueue::removeCallback(std::size_t id) {
    std::lock_guard<std::mutex> lock(mtx);
    int indexToErase = -1;
    for (int i = 0; i < callbacks.size(); i++) {
        if (id == callbacks[i].id) {
            indexToErase = i;
            break;
        }
    }

    std::vector<Pair>::iterator it = callbacks.begin();
    std::advance(it, indexToErase);
    callbacks.erase(it);
}
