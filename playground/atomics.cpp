#include <atomic>
#include <thread>
#include <cassert>

std::atomic<int> x;


void store() {
    x.store(10, std::memory_order_relaxed);
    x.store(20, std::memory_order_relaxed);
}

void load() {
    int y = x.load(std::memory_order_relaxed);
    int z = x.load(std::memory_order_relaxed);
    assert(y <= x);
}

int main() {
    std::thread t1, t2;
    t2 = std::thread(load);
    t1 = std::thread(store);
    t1.join();
    t2.join();
    return 0;
}