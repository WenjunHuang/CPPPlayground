//
// Created by HUANG WEN JUN on 2020/2/27.
//
#include <boost/asio.hpp>
#include <iostream>

int main() {
    boost::asio::io_service ioService;
    boost::asio::io_service::work work{ioService};

    ioService.run();
    std::cout << "Do you reckon this line" << std::endl;
}