//
// Created by HUANG WEN JUN on 2019/10/12.
//
#include <QString>
#include <iostream>
#include <optional>
#include <string>
#include <variant>
template <typename T, typename Variant>
std::optional<T> get_if(const Variant& variant) {
    T* ptr = std::get_if<T>(&variant);
    if (ptr) {
        return *ptr;
    } else {
        return std::optional<T>();
    }
}
class init_t {};

class running_t {
  public:
    unsigned count() const { return _count; }

  private:
    unsigned _count;
};

class finished_t {
  public:
    finished_t(unsigned count) : _count{count} {}

  private:
    unsigned _count;
};

class program_t {
  public:
    program_t() : _state{init_t()} {}

    void counting_finished() {
        auto state = get_if<running_t>(&_state);
        _state     = finished_t(state->count());
    }

  private:
    std::variant<init_t, running_t, finished_t> _state;
};

int main() { std::string s1{"abc"}; }