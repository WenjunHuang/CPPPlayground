//
// Created by rick on 2021/5/17.
//
#include <string>
#include <vector>
template <typename T>
struct Monoid {
    static T op(T a1, T a2);
    static T zero();
};
template <>
struct Monoid<std::string> {
    static std::string op(const std::string& a1, const std::string& a2)
    {
        return a1 + a2;
    }
    static std::string zero()
    {
        return "";
    }
};

template <typename T>
struct Monoid<std::vector<T>> {
    static std::vector<T> op(std::vector<T> a1, std::vector<T> a2)
    {
        a1.insert(a1.end(), std::make_move_iterator(a2.begin()), std::make_move_iterator(a2.end()));
        return std::move(a1);
    }
    static std::vector<T> zero()
    {
        return std::vector<T> {};
    }
};
int main()
{
    auto result = Monoid<std::vector<std::string>>::op(std::vector<std::string> { "a", "b" }, std::vector<std::string> { "c", "d" });
}