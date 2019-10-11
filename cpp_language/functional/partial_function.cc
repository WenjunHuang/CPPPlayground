
#include <algorithm>
#include <functional>
#include <utility>
#include <vector>
#include <cmath>
template <typename Function, typename SecondArgType>
class partial_application_on_2nd_impl {
  public:
    partial_application_on_2nd_impl(Function function, SecondArgType secondArg)
        : _function(function), _secondArg(secondArg) {}

    template <typename FirstArgType>
    decltype(auto) operator()(FirstArgType&& firstArg) const {
        return _function(std::forward<FirstArgType>(firstArg), _secondArg);
    }

  private:
    Function _function;
    SecondArgType _secondArg;
};

template <typename Function, typename SecondArgType>
partial_application_on_2nd_impl<Function, SecondArgType>
bind2nd(Function&& function, SecondArgType&& secondArg) {
    return partial_application_on_2nd_impl<Function, SecondArgType>(
        std::forward<Function>(function),
        std::forward<SecondArgType>(secondArg));
}

int main() {
    std::vector<double> degrees = {0, 30, 45, 60};
    std::vector<double> radians(degrees.size());

    std::transform(degrees.cbegin(), degrees.cend(), radians.begin(),
                   bind2nd(std::multiplies<>(), M_PI / 180));
}