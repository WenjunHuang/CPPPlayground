//
// Created by rick on 2019/8/30.
//

#pragma once
#include <QtGlobal>
#include <type_traits>

#if (QT_POINTER_SIZE == 8)
#define QBT_APP_64BIT
#endif

const char kTorrentFileExtension[] = ".torrent";
const int kMaxTorrentSize          = 100 * 1024 * 1024;

template <typename T>
constexpr typename std::add_const<T>::type& asConst(T& t) noexcept {
    return t;
}

template <typename T>
constexpr typename std::add_const<T>::type& asConst(T&& t) noexcept {
    return std::forward<T>(t);
}

// Prevent const rvalue arguments
template <typename T>
void asConst(const T &&) = delete;
