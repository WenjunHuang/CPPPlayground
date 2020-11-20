//
// Created by rick on 2020/11/20.
//
#include <rx.hpp>
#include <mutex>

std::mutex printMutex;

int main() {
    rxcpp::rxsub::subject<int> sub;
    auto subscriber = sub.get_subscriber();
    auto observable1 = sub.get_observable();
    auto observable2 = sub.get_observable();

    auto onNext = [](int v){
        std::lock_guard<std::mutex> lock{printMutex};
        std::cout << "Observable Thread id => "
        << std::this_thread::get_id()
        << "\t OnNext: " << v << std::endl;
    };
    auto onError = [](std::exception_ptr ep){
        std::lock_guard<std::mutex> lock{printMutex};
        std::cout << "Observable Thread id => "
        << "t OnError: "
        << rxcpp::util::what(ep).c_str()
        << std::endl;
    };

    observable1.observe_on(rxcpp::synchronize_new_thread())
    .subscribe(onNext,onError,[](){printf("OnCompleted\n");});
    observable2.observe_on(rxcpp::synchronize_new_thread())
        .subscribe(onNext,onError,[](){printf("OnCompleted\n");});

    for (int i = 1;i <=10;i++){
        if (i > 5){
            try{
                std::string().at(1);
            }catch(...){
                std::exception_ptr eptr = std::current_exception();
                subscriber.on_error(eptr);
                break;
            }
        }
        subscriber.on_next(i * 10);
    }
    subscriber.on_completed();

    rxcpp::observable<>::timer(std::chrono::milliseconds(2000))
    .subscribe([&](long){});
}
