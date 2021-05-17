//
// Created by HUANG WEN JUN on 2020/11/18.
//

#include <array>
#include <iostream>
#include <rx.hpp>

using namespace rxcpp;

/**
* The LiftAction class converts an Action (a lambda)
 * and wraps it into a form which can help us to connect
 * to an observable<T> using the observable<T>::lift<T> method.
*/
template <typename Action>
struct LiftAction {
    typedef std::decay_t<Action> action_type;
    action_type action;

    LiftAction(action_type t)
        : action { t }
    {
    }

    template <typename Subscriber>
    struct action_observer : observer_base<typename std::decay_t<Subscriber>::value_type> {
        typedef action_observer<Subscriber> this_type;
        typedef rxcpp::observer_base<typename std::decay_t<Subscriber>::value_type> base_type;

        typedef std::decay_t<Subscriber> dest_type;
        typedef typename std::decay_t<Subscriber> dest_type;
        typedef typename base_type::value_type value_type;
        typedef rxcpp::observer<value_type,this_type> observer_type;

        dest_type dest;
        action_type action;
        action_observer(dest_type d, action_type a)
            : dest { d }
            , action { a }
        {
        }

        void on_next(typename dest_type::value_type v) const
        {
            dest.on_next(action(v));
        }

        void on_error(std::exception_ptr e)const {
            dest.on_error(e);
        }

        void on_completed() const {
            dest.on_completed();
        }

        static rxcpp::subscriber<value_type,observer_type>
            make(const dest_type&d,const action_type& t) {
            return rxcpp::make_subscriber<value_type>(d,observer_type{this_type{d,t}});
        }
    };

    template<class Subscriber>
    auto operator()(const Subscriber& dest) const -> decltype(action_observer<Subscriber>::make(dest,action)) {
        return action_observer<Subscriber>::make(dest,action);
    }
};

template<class Action>
auto liftAction(Action&& p)->LiftAction<std::decay_t<Action>> {
    return LiftAction<std::decay_t<Action>>(std::forward<Action>(p));
}

int main()
{
    std::array<int, 4> a = { { 10, 20, 30, 40 } };
    auto h = [](observable<int> src) {
        return src.map([](int n) { return n; });
    };
    auto g = [](observable<int> src) {
        return src.map([](int n) { return n * 2; });
    };

    auto values = g(h(observable<>::iterate(a)))
                      .lift<int>(liftAction([](int r) { return 2 * r; }));
    // as usual subscribe
    values.subscribe([](int f) { std::cout << f << std::endl; },
        []() { std::cout << "Hello World..." << std::endl; });
}