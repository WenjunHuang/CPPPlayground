//
// Created by rick on 2020/11/19.
//

#include <rx.hpp>

int main() {
    rxcpp::rxsub::subject<int> sub;
    auto subscriber = sub.get_subscriber();
    auto observable = sub.get_observable();

    observable.subscribe([](int v) { printf("OnNext: %d\n", v); },
                         [](std::exception_ptr ep) {
                             printf("OnError: %s\n",
                                    rxcpp::util::what(ep).c_str());
                         },
                         []() { printf("OnCompleted\n"); });

    for (int i = 0;i<=10;i++){
        if (i > 5){
            try {
                std::string().at(1);
            }catch(std::out_of_range& ex){
                subscriber.on_error(std::make_exception_ptr(ex));
                break;
            }
        }
        subscriber.on_next(i * 10);
    }
    subscriber.on_completed();

}