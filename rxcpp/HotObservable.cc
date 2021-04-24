//
// Created by HUANG WEN JUN on 2020/11/17.
//
#include <memory>
#include <rx.hpp>
#include <thread>

int main()
{
    std::mutex mutex;
    using lock_guard = std::lock_guard<std::mutex>;

    std::cout << "main thread id: " << std::this_thread::get_id() << '\n';
    auto eventLoop = rxcpp::observe_on_event_loop();
    auto worker = rxcpp::observe_on_new_thread();
    auto values = rxcpp::observable<>::interval(std::chrono::seconds(1),eventLoop)
                      .take(3)
                      .publish();

    values.observe_on(worker).subscribe(
        [&](int v) {
            lock_guard guard{mutex};
            std::cout << "[0] onNext: " << v
                      << " thread:" << std::this_thread::get_id() << std::endl;
        },
        [&]() {
            lock_guard guard{mutex};
            printf("[0] onCompleted\n"); });
    values.observe_on(worker).subscribe(
        [&](int v) {
            lock_guard guard{mutex};
            std::cout << "[1] onNext: " << v
                      << " thread:" << std::this_thread::get_id() << std::endl;
        },
        [&]() {
            lock_guard guard{mutex};
            printf("[1] onCompleted\n"); });
    values.subscribe(
        [&](int v) {
            lock_guard guard{mutex};
            std::cout << "[2] onNext: " << v
                      << " thread:" << std::this_thread::get_id() << std::endl;
        },
        [&]() {
            lock_guard guard{mutex};
            printf("[2] onCompleted\n"); });
    values.subscribe(
        [&](int v) {
            lock_guard guard{mutex};
            std::cout << "[3] onNext: " << v
                      << " thread:" << std::this_thread::get_id() << std::endl;
        },
        [&]() {
            lock_guard guard{mutex};
            printf("[3] onCompleted\n"); });

    values.connect();
    values.as_blocking().subscribe();
    //    rxcpp::observable<>::timer(std::chrono::milliseconds(2000))
    //        .subscribe([&](long) {});
}
