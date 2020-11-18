//
// Created by HUANG WEN JUN on 2020/11/18.
//

#include <rx.hpp>
#include <type_traits>

template <typename Duration,
          typename T = std::enable_if<std::is_copy_assignable_v<Duration>>>
auto generateObservable(Duration duration) {
    auto start  = rxcpp::identity_current_thread().now();
    auto period = duration;
    return rxcpp::observable<>::interval(start, period).take(3);
}

int main() {
    auto coordination = rxcpp::observe_on_event_loop();
    auto worker       = coordination.create_coordinator().get_worker();
    auto values       = generateObservable(std::chrono::milliseconds(2))
                      .replay(2, coordination);

    worker.schedule([&](const rxcpp::schedulers::schedulable&) {
        values.subscribe(
            [](long v) {
                printf("#1 -- %d: %ld\n", std::this_thread::get_id(), v);
            },
            []() { printf("#1 --- OnCompleted\n"); });
    });

    worker.schedule([&](const rxcpp::schedulers::schedulable&){
        values.subscribe([](long v){printf("#2 -- %d: %ld\n",std::this_thread::get_id(),v);},
                         [](){printf("#2 --- OnCompleted\n");});
    });

    worker.schedule([&](const rxcpp::schedulers::schedulable&){
        values.connect();
    });

    values.as_blocking().subscribe();
}