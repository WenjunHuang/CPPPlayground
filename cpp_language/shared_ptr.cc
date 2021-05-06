//
// Created by rick on 2021/4/29.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <memory>
#include <string>
#include <vector>
#include <cassert>
#include <doctest/doctest.h>
struct Widget{
    std::vector<std::string> locations;
};
TEST_CASE("weak_ptr should not increase reference count"){
    auto spw = std::make_shared<Widget>();
    std::weak_ptr<Widget> wpw{spw};
    CHECK(spw.use_count() == 1);

    {
        auto spw2 = wpw.lock();
        CHECK(spw2);
    }


    spw = nullptr;
    CHECK(wpw.expired());

    auto spw2 = wpw.lock();
    CHECK(!spw2);
}