#include <iostream>
#include <variant>
#include <vector>

class NotSimple {
  public:
    NotSimple(int, float) {}
};
int main() {
    std::variant<int, float> intFloat;
    std::cout << intFloat.index() << ", val:" << std::get<int>(intFloat)
              << '\n';

    std::variant<std::monostate, NotSimple, int> okInit;
    std::cout << okInit.index() << '\n';

    // pass a value
    std::variant<int, float, std::string> intFloatString{10.5f};
    std::cout << intFloatString.index() << ", value "
              << std::get<float>(intFloatString) << '\n';

    // ambiguity resolved by in_place
    std::variant<long, float, std::string> longFloatString{
        std::in_place_index<1>, 7.6};
    std::cout << longFloatString.index() << ", value "
              << std::get<float>(longFloatString) << '\n';

    // in_place for complex types
    std::variant<std::vector<int>, std::string> vecStr{std::in_place_index<0>,
                                                       {0, 1, 2, 3, 4, 5}};
    std::cout << vecStr.index() << ", vector size "
              << std::get<std::vector<int>>(vecStr).size() << '\n';

    // copy-initialize from other variant:
    std::variant<int, float> intFloatSecond{intFloat};
    std::cout << intFloatSecond.index() << ", value "
              << std::get<int>(intFloatSecond) << '\n';
}
