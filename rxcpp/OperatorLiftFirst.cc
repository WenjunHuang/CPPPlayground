//
// Created by HUANG WEN JUN on 2020/11/18.
//

#include <array>
#include <iostream>
#include <rx.hpp>

template <class Action> struct LiftAction {
    typedef typename std::decay<Action>::type action_type;
    action_type action;

    LiftAction(action_type t) : action{t} {}

    template <class Subscriber>
    struct action_observer
        : public rxcpp::observer_base<
              typename std::decay<Subscriber>::type::value_type> {
        typedef action_observer<Subscriber> this_type;
        typedef rxcpp::observer_base<
            typename std::decay<Subscriber>::type::value_type>
            base_type;
        typedef typename base_type::value_type value_type;
        typedef typename std::decay<Subscriber>::type dest_type;
        typedef rxcpp::observer<value_type, this_type> observer_type;

        dest_type dest;
        action_type action;
        action_observer(dest_type d, action_type t) : dest{d}, action{t} {}

        void on_next(typename dest_type::value_type v) const {
            dest.on_next(action(v));
        }

        void on_error(std::exception_ptr e) const { dest.on_error(e); }

        void on_completed() const { dest.on_completed(); }

        static rxcpp::subscriber<value_type, observer_type>
        make(const dest_type& d, const action_type& t) {
            return rxcpp::make_subscriber<value_type>(
                d, observer_type(this_type(d, t)));
        }
    };

    template<class Subscriber>
    auto operator()(const Subscriber& dest) const ->decltype(action_observer<Subscriber>::make(dest,action)) {
        return action_observer<Subscriber>::make(dest,action);
    }
};

template<class Action>
auto liftAction(Action&& p) -> LiftAction<typename std::decay<Action>::type>{
    return LiftAction<typename std::decay<Action>::type>(std::forward<Action>(p));
}

int main() {
    std::array<int, 4> a{10, 20, 30, 40};
    auto lambdaFn = [](int n) { return "Str:" + std::to_string(n * 2); };


    auto transform = [=](rxcpp::subscriber<std::string> dest) {
        return rxcpp::make_subscriber<int>(
            dest, rxcpp::make_observer_dynamic<int>(
                      [=](int n) { dest.on_next(lambdaFn(n)); },
                      [=](std::exception_ptr e) { dest.on_error(e); },
                      [=]() { dest.on_completed(); }));
    };

    auto values = rxcpp::observable<>::iterate(a);
    values.lift<std::string>(transform).subscribe(
        [](std::string f) { std::cout << f << std::endl; },
        []() { std::cout << "End.." << std::endl; });
}
