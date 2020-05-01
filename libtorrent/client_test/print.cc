//
// Created by rick on 2020/4/5.
//
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#endif
#include "print.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <libtorrent/config.hpp>

std::string esc(std::string const& code) {
    std::string ret{"\033["};
    ret.append(code);
    ret.append("m");
    return ret;
}

std::string to_string(int v, int width) {
    std::array<char, 100> buf{};
    std::snprintf(buf.data(), buf.max_size(), "%*d", width, v);
    return buf.data();
}

std::string add_suffix_float(double val, const std::string& suffix) {
    if (val < 0.001) {
        std::string ret;
        ret.resize(4 + 2, ' ');
        if (!suffix.empty())
            ret.resize(4 + 2 + suffix.length(), ' ');

        return ret;
    }

    std::array<std::string, 5> prefix{"kB", "MB", "GB", "TB", "PB"};
    auto num_prefix = prefix.size();
    int i           = 0;
    for (; i < num_prefix - 1; ++i) {
        val /= 1000.;
        if (std::fabs(val) < 1000.)
            break;
    }

    std::array<char, 100> ret{};
    std::snprintf(ret.data(), ret.max_size(), "%4.*f%s%s", val < 99 ? 1 : 0,
                  val, prefix[i].data(), suffix.empty() ? "" : suffix.data());
    return ret.data();
}

std::string color(std::string const& s, color_code c) {
    if (c == col_none)
        return s;

    if (std::count(s.begin(), s.end(), ' ') == int(s.size()))
        return s;

    std::array<char, 1024> buf{};
    std::snprintf(buf.data(), buf.max_size(), "\x1b[3%dm%s\x1b[39m", c,
                  s.c_str());
    return buf.data();
}

std::string const& progress_bar(int progress,int width,color_code c,
    char fill,char bg,std::string caption,int flags) {
    static std::string bar;
    bar.clear();
    bar.reserve(size_t(width + 10));

    auto const progress_chars = static_cast<std::size_t>((progress * width + 500) / 1000);

    if (caption.empty()) {
        std::array<char,10> code{};
        std::snprintf(code.data(),code.max_size(),"\x1b[3%dm",c);

        bar = code.data();
        std::fill_n(std::back_inserter(bar),progress_chars,fill);
        std::fill_n(std::back_inserter(bar),width - progress_chars,bg);
        bar += esc("39");
    } else {
        color_code tc = col_black;
        if (c == col_black || c == col_blue)
            tc = col_white;

        caption.resize(size_t(width), ' ');
#ifdef _WIN32
        char const* background = "40";
#endif
        std::array<char,256> str{};
        if (flags & progress_invert) {
            std::snprintf(str.data(),str.max_size(),"\x1b[%sm\x1b[37m%s\x1b[4%d;3%dm%s\x1b[49;39m",
                background,
                caption.substr(0,progress_chars).c_str(),
                c,tc,caption.substr(progress_chars).c_str());
        } else {
            std::snprintf(str.data(),
                str.max_size(),"\x1b[4%d;3%dm%s\x1b[%sm\x1b[37m%s\x1b[49;39m",
                c,tc,caption.substr(0,progress_chars).c_str(),
                background,caption.substr(progress_chars).c_str());
        }

        bar = str.data();
    }
    return bar;
}