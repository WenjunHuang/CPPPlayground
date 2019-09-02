//
// Created by rick on 2019/8/31.
//
#pragma once
#include <type_traits>

namespace Algorithm {
    template <typename...> using void_t = void;

    template <typename T, typename = void>
    struct HasMappedType : std::false_type {};

    template <typename T>
    struct HasMappedType<T, void_t<typename T::mapped_type>> : std::true_type {
    };

    // To be used with associative array types
    template <typename T, typename BinaryPredicate,
              typename std::enable_if_t<HasMappedType<T>::value, int> = 0>
    void removeIf(T& dict, BinaryPredicate&& p) {
        auto it = dict.begin();
        while (it != dict.end())
            it = (p(it.key(), it.value()) ? dict.erase(it) : (it + 1));
    }

    template <typename T, typename UnaryPredicate,
              typename std::enable_if_t<!HasMappedType<T>::value, int> = 0>
    void removeIf(T& set, UnaryPredicate&& p) {
        auto it = set.begin();
        while (it != set.end())
            it = (p(*it) ? set.erase(it) : it + 1);
    }

}