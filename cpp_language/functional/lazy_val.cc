//
// Created by HUANG WEN JUN on 2019/10/12.
//
#include <mutex>

template <typename F> class lazy_val {
  private:
    F _computation;
    mutable decltype(_computation()) _cache;
    mutable std::once_flag _valueFlag;

  public:
    lazy_val(F computation) : _computation{computation} {}

    operator const decltype(_computation())&() const {
        std::call_once(_valueFlag, [this] { _cache = _computation(); });

        return _cache;
    }
};

template <typename F> inline lazy_val<F> make_lazy_val(F&& computation) {
    return lazy_val<F>(std::forward<F>(computation));
}

int main() {}