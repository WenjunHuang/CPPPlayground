//
// Created by rick on 2021/4/27.
//

#pragma once

#include <rx.hpp>
#include <type_traits>
using namespace rxcpp;

namespace rxcpp {
struct eval_tag {
    template <class Included>
    struct include_header {
        static_assert(Included::value, "missing include: please #include <rxcpp/operators/rx-eval.hpp>");
    };
};
}

namespace rxcpp::operators::detail {
template <class... AN>
struct eval_invalid_arguments {
};

template <class... AN>
struct eval_invalid : public rxo::operator_base<eval_invalid_arguments<AN...>> {
    using type = observable<eval_invalid_arguments<AN...>, eval_invalid<AN...>>;
};

template <class... AN>
using eval_invalid_t = typename eval_invalid<AN...>::type;

template <class T, class Selector>
struct eval {
    typedef std::decay_t<T> source_value_type;
    typedef std::decay_t<Selector> select_type;
    typedef decltype((*(select_type*)nullptr)(*(source_value_type*)nullptr)) value_type;
    select_type selector;

    eval(select_type s)
        : selector(std::move(s))
    {
    }

    template <class Subscriber>
    struct eval_observer {
        typedef eval_observer<Subscriber> this_type;
        typedef decltype((*(select_type*)nullptr)(*(source_value_type*))) value_type;
        typedef std::decay_t<Subscriber> dest_type;
        typedef observer<source_value_type, this_type> observer_type;
        dest_type dest;
        mutable select_type selector;

        eval_observer(dest_type d, select_type s)
            : dest { std::move(d) }
            , selector { std::move(s) }
        {
        }

        template <class Value>
        void on_next(Value&& v) const
        {
            auto selected = on_exception([&]() {
                return this->selector(std::forward<Value>(v));
            },dest);
            if (selected.empty()){return;}
            dest.on_next(std::move(selected.get()));
        }

        void on_error(std::exception_ptr e) const {
            dest.on_error(e);
        }

        void on_completed() const {
            dest.on_completed();
        }

        static subscriber<source_value_type , observer_type> make(dest_type d,select_type s){
            auto cs = d.get_subscription();
            return make_subscriber<source_value_type>(std::move(cs),observer_type(this_type(std::move(d),std::move(s))));
        }
    };

    template<class Subscriber>
    auto operator()(Subscriber dest) const -> decltype(eval_observer<Subscriber>::make(std::move(dest),selector)) {
        return eval_observer<Subscriber>::make(std::move(dest),selector);
    }
};
}

namespace rxcpp {
namespace operators {
    template <class... AN>
    auto eval(AN&&... an) -> operator_factory<eval_tag, AN...>
    {
        return operator_factory<eval_tag, AN...>(std::make_tuple(std::forward<AN>(an)...));
    }
}

template<>
struct member_overload<eval_tag>{
    template<class Observable,class Selector,
        class Enabled=std::enable_if_t<is_observable<Observable>>,
        class ResolvedSelector = std::decay_t<Selector>,
        class SourceValue = rxu::value_type_t<Observable>,
        class Eval = rxo::detail::eval<SourceValue,ResolvedSelector>,
        class Value = rxu::value_type_t<Eval>>
    static auto member(Observable&& o,Selector&& s) -> decltype(o.template lift<Value>(Eval(std::forward<Selector>(s)))){
        return o.template lift<Value>(Eval(std::forward<Selector>(s)));
    }

    template<class... AN>
    static operators::detail::eval_invalid_t<AN...> member(const AN...) {
        std::terminate();
        return {};
        static_assert(sizeof...(AN) == 100000,"eval takes Selector");
    }
};

}