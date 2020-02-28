//
// Created by rick on 2020/2/26.
//
#include <experimental/coroutine>
#include <boost/asio.hpp>

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <cstdio>
#include <iostream>

using boost::asio::ip::tcp;
namespace asio = boost::asio;

asio::awaitable<void> echo(asio::ip::tcp::socket socket) {
    try {
        char data[1024];
        for (;;) {
            std::size_t n = co_await socket.async_read_some(
                asio::buffer(data), asio::use_awaitable);
            co_await async_write(socket, asio::buffer(data, n),
                                 asio::use_awaitable);
        }
    } catch (std::exception& e) {
        std::cout << "echo Exception: " << e.what() << std::endl;
    }
}

asio::awaitable<void> listener() {
    auto executor = co_await asio::this_coro::executor;
    tcp::acceptor acceptor{executor, {tcp::v4(), 55555}};
    for (;;) {
        tcp::socket socket =
            co_await acceptor.async_accept(asio::use_awaitable);
        asio::co_spawn(
            executor,
            [socket = std::move(socket)]() mutable {
                return echo(std::move(socket));
            },
            asio::detached);
    }
}

int main() {
    try {
        asio::io_context ioContext{1};
        asio::signal_set signals(ioContext,SIGINT,SIGTERM);
        signals.async_wait([&](auto,auto){ioContext.stop();});

        co_spawn(ioContext,listener,asio::detached);
        ioContext.run();
    }catch (std::exception & e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}
