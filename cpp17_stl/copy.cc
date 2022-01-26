//
// Created by HUANG WEN JUN on 2021/12/29.
//
#include <cstdio>
#include <boost/iterator/iterator_facade.hpp>
#include <string>

class PutcIterator : public boost::iterator_facade<
        PutcIterator,
        const PutcIterator,
        std::output_iterator_tag> {
    friend class boost::iterator_core_access;

    auto &dereference() const { return *this; }

    void increment() {}

    bool equal(const PutcIterator &) const { return false; }

public:
    void operator=(char ch) const { std::putc(ch, stdout); }
};

int main() {
    std::string s = "hello";
    std::copy(s.begin(), s.end(), PutcIterator());

}

