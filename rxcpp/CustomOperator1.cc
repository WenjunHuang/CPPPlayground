//
// Created by HUANG WEN JUN on 2020/11/18.
//

#include <rx.hpp>
namespace rx{
    using namespace rxcpp;
    using namespace rxcpp::operators;
    using namespace rxcpp::sources;
    using namespace rxcpp::util;
}
using namespace rx;

template<typename Container>
rx::observable<std::string> helloNames(Container items){
    auto str = rx::observable<>::iterate(items);
    return str.filter([](const std::string& s){
        return s.length() > 5;
    })
    .map([](std::string s){
            return "Hello, " + s + "!";
        })
    .on_error_resume_next([](std::exception_ptr){
            return rx::error<std::string>(std::runtime_error("custom exception"));
        });
}

int main() {
    std::array<std::string,3> names{"Praseed","Peter","Joseph"};
    auto value = helloNames(names).take(2);
    auto errorHandler = [=](std::exception_ptr e){
        try{
            std::rethrow_exception(e);
        }catch(const std::exception& ex){
            std::cerr << ex.what() << std::endl;
        }
    };
    value.subscribe([](const std::string&s ){std::cout << s << std::endl;},
                    errorHandler,
                    [](){std::cout<<"OnCompleted" << std::endl;});
}