//
// Created by rick on 2020/11/19.
//

#include <rx.hpp>

int main() {
    auto values =
        rxcpp::observable<>::range(1, 3)
            .concat(rxcpp::observable<>::error<int>(
                std::runtime_error("Error from producer!")))
            .on_error_resume_next([](std::exception_ptr ep) {
                printf("Resuming after: %s\n", rxcpp::util::what(ep).c_str());
                return rxcpp::observable<>::just(-1);
            });

    values.subscribe([](int v){printf("OnNext: %d\n",v);},
                     [](std::exception_ptr ep){
                         printf("OnError: %s\n",rxcpp::util::what(ep).c_str());
                     },[](){printf("OnCompleted\n");});
}