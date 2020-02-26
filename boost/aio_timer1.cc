//
// Created by rick on 2020/2/26.
//
#include <boost/asio.hpp>
#include <iostream>
using namespace boost::asio;

int main() {
    io_context io;
    steady_timer t{io, chrono::seconds(5)};
    int count  = 0;
    auto print = [&t, &count](const boost::system::error_code& error) mutable {
        auto worker = [&t, &count](const boost::system::error_code& error,
                                   auto& workerRef) mutable {
            if (count < 5) {
                std::cout << count << std::endl;
                ++count;

                t.expires_at(t.expiry() + chrono::seconds(1));
                t.async_wait(
                    [&workerRef](auto& error) { workerRef(error, workerRef); });
            } else {
                std::cout << "Final count is " << count << std::endl;
            }
        };
        worker(error, worker);
    };
    t.async_wait(print);
    io.run();
    std::cout << "End" << std::endl;
}