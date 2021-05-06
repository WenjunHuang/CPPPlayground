//
// Created by rick on 2020/3/24.
//
#include <rx.hpp>
int main() {
    std::cout << "Main Thread: " << std::this_thread::get_id() << '\n';
    auto coordination = rxcpp::serialize_new_thread();
    auto worker = coordination.create_coordinator()
    .get_worker();
    auto subAction = rxcpp::schedulers::make_action([](const rxcpp::schedulers::schedulable&){
        printf("Action Executed in Thread #: %d\n",std::this_thread::get_id());
    });

    auto scheduled = rxcpp::schedulers::make_schedulable(worker,subAction);
    scheduled.schedule();

    std::this_thread::sleep_for(std::chrono::seconds(2));
}