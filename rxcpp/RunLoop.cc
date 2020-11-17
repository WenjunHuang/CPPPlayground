//
// Created by HUANG WEN JUN on 2020/11/17.
//
#include <rx.hpp>
#include <thread>
int main() {
    printf("Main Thread id is %d\n",std::this_thread::get_id());

    rxcpp::schedulers::run_loop rlp;

    auto mainThread = rxcpp::observe_on_run_loop(rlp);
    auto workerThread = rxcpp::synchronize_new_thread();
    rxcpp::composite_subscription scr;
    rxcpp::observable<>::range(0,15)
    .map([](int i){
            printf("Map %d: %d\n",std::this_thread::get_id(),i);
            return i;
        })
    .take(5)
    .subscribe_on(workerThread)
    .observe_on(mainThread)
    .subscribe(scr,[&](int i){
            printf("Sub %d: %d\n",
                   std::this_thread::get_id(),i);
        });
    while(scr.is_subscribed()||!rlp.empty()){
        while(!rlp.empty() && rlp.peek().when < rlp.now())
            rlp.dispatch();
    }
}
