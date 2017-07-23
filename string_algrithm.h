//
// Created by xxzyjy on 23/07/2017.
//

#ifndef BOOSTPLAYGROUND_STRING_ALGRITHM_H
#define BOOSTPLAYGROUND_STRING_ALGRITHM_H

#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>

using namespace boost::algorithm;

void string_algorithm() {
    std::string s = "Boost C++ Libraries";
    std::cout << to_upper_copy(s) << '\n';
}

#endif //BOOSTPLAYGROUND_STRING_ALGRITHM_H
