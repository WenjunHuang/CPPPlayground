//
// Created by rick on 2020/3/24.
//
#include <rx.hpp>
int main() {
    std::cout << "Main thread#: "  << std::this_thread::get_id() << '\n';
    auto coordination = rxcpp::serialize_new_thread();
    auto worker = coordination.create_coordinator().get_worker();
    int count = 10;
    auto subAction = rxcpp::schedulers::make_action([count](const rxcpp::schedulers::schedulable& sch)mutable {
        std::cout <<"Action Executed in Thread #: " << std::this_thread::get_id() << '\n';
        if (count-- != 0)
            sch();
    });

    auto scheduled = rxcpp::schedulers::make_schedulable(worker,subAction);
    scheduled.schedule();

    std::this_thread::sleep_for(std::chrono::seconds(2));
}