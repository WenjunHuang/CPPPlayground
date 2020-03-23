//
// Created by rick on 2020/3/12.
//
#include <atomic>
#include <cassert>
#include <thread>

std::atomic<int> data[5];
std::atomic<bool> sync1{false},sync2{false};

void thread_1() {
    data[0].store(42,std::memory_order_relaxed);
    data[1].store(97,std::memory_order_relaxed);
    data[2].store(17,std::memory_order_relaxed);
    data[3].store(-141,std::memory_order_relaxed);
    data[4].store(2003,std::memory_order_relaxed);
    sync1.store(true,std::memory_order_release);
}

void thread_2() {
    while (!sync1.load(std::memory_order_acquire));
    sync2.store(true,std::memory_order_release);
}

void thread_3() {
    while(!sync2.load(std::memory_order_acquire));

    assert(data[0].load(std::memory_order_relaxed) == 42);
    assert(data[1].load(std::memory_order_relaxed) == 97);
}

int main() {
    std::thread thread1{thread_1};
    std::thread thread2{thread_2};
    std::thread thread3{thread_3};

    thread1.join();
    thread2.join();
    thread3.join();

}