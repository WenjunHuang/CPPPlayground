//
// Created by HUANG WEN JUN on 2020/1/20.
//
#include <atomic>
class spinlock_mutex {
    std::atomic_flag flag_;
  public:
    spinlock_mutex():flag_{ATOMIC_FLAG_INIT}{}

    void lock() {
        while(flag_.test_and_set(std::memory_order_acquire));
    }

    void unlock() {
        flag_.clear(std::memory_order_release);
    }
};
int main() {
    std::atomic_flag f = ATOMIC_FLAG_INIT;
    f.clear(std::memory_order_release);
    bool x = f.test_and_set();
}