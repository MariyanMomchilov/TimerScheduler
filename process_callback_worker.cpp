#include "./process_callback_worker.h"

void processCallbackWorker(CallbackQueue &queue, std::condition_variable &event, std::atomic_size_t &callbackId, std::vector<Log> *logs) {
    std::unique_lock<std::mutex> lock(queue.mtx);
    while(!queue.closed.load(std::memory_order_relaxed)) {
        if (!lock.owns_lock()) {
            lock.lock();
        }

        event.wait(lock, [&queue]() -> bool {
            return queue.closed.load(std::memory_order_relaxed) || queue.callbacks.size() > 0;
        });

        if (queue.callbacks.size() > 0) {
            CallbackQueue::Pair pair = queue.callbacks[0];
            queue.callbacks.erase(queue.callbacks.begin());
            callbackId.store(pair.id, std::memory_order_relaxed);
            lock.unlock();
            if (logs != nullptr) {
                (*logs)[pair.id].executed.push_back(std::chrono::system_clock::now());
            }
            pair.cb();
            callbackId.store(0, std::memory_order_release);
        } else {
            lock.unlock();
        }
    }
}
