//
// Created by rick on 2021/4/28.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "rxcurl.h"
#include <doctest/doctest.h>
#include <iostream>
#include <thread>

TEST_CASE("test")
{
    using namespace rxcurl;
    // start two threads
    auto rxCurl = createRxCurl();
    std::thread thread1([&](){
        HttpRequest request {
            "https://news.sina.com.cn",
            "get",
        };
        rxCurl.create(request)
            .flat_map([](const HttpResponse& response) { return response.body.complete; }, [](const HttpResponse& response, const std::string& body) { return std::make_tuple(response, body); })
            .as_blocking()
            .subscribe([](std::tuple<HttpResponse, std::string> response) {
                std::cout << std::get<0>(response).code() << std::endl;
                std::cout << std::get<0>(response).httpStatus() << std::endl;
//                std::cout << std::get<1>(response) << std::endl;
                //            std::cout << response.httpStatus() << std::endl;
                //            std::cout << response.code() << std::endl;
            });
    });

    std::thread thread2([&](){
        HttpRequest request {
            "https://music.163.com",
            "get",
        };
        rxCurl.create(request)
            .flat_map([](const HttpResponse& response) { return response.body.complete; }, [](const HttpResponse& response, const std::string& body) { return std::make_tuple(response, body); })
            .as_blocking()
            .subscribe([](std::tuple<HttpResponse, std::string> response) {
                std::cout << std::get<0>(response).code() << std::endl;
                std::cout << std::get<0>(response).httpStatus() << std::endl;
//                std::cout << std::get<1>(response) << std::endl;
                //            std::cout << response.httpStatus() << std::endl;
                //            std::cout << response.code() << std::endl;
            });

    });
    thread1.join();
    thread2.join();
}