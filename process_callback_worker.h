#pragma once
#include <condition_variable>
#include <atomic>
#include <vector>
#include "./callback_queue.h"
#include "./log.h"

void processCallbackWorker(CallbackQueue &queue, std::condition_variable &event, std::atomic_size_t &callbackId, std::vector<Log> *logs = nullptr);
