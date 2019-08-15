//
// Created by xxzyjy on 30/07/2017.
//

#include <gtest/gtest.h>
#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <array>
#include <thread>
#include <chrono>
#include <iostream>

using namespace boost::asio;
using namespace std;

TEST(Asio, steady_timer) {
  io_service ioservice;
  steady_timer timer {ioservice, chrono::seconds{3}};
  timer.async_wait([](const boost::system::error_code &ec){
    cout << "3 sec" <<endl;
  });
  steady_timer timer2 {ioservice, chrono::seconds{4}};
  timer2.async_wait([](const boost::system::error_code &ec){
    cout << "4 sec" <<endl;
  });
  ioservice.run();
}

TEST(Asio, multithreads) {
  io_service ioservice;
  steady_timer timer1{ioservice,chrono::seconds{3}};
  timer1.async_wait([](const boost::system::error_code &ec){
    cout << "3 sec" <<endl;
  });

  steady_timer timer2{ioservice,chrono::seconds{3}};
  timer2.async_wait([](const boost::system::error_code &ec) {
    cout << "3 sec" <<endl;
  });

  thread thread1([&ioservice](){
    ioservice.run();
  });
  thread thread2([&ioservice](){
    ioservice.run();
  });

  thread1.join();
  thread2.join();

}

TEST(Asio,network) {
  using namespace boost::asio;
  using namespace boost::asio::ip;

  io_service ioservice;
  tcp::resolver resolver{ioservice};
  tcp::socket tcp_socket{ioservice};
  array<char, 4096> bytes;

  function<void (const boost::system::error_code&, size_t)> read_handler =
    [&](const boost::system::error_code &ec, size_t bytesTransferred) {
      if (!ec) {
        cout.write(bytes.data(),bytesTransferred);
        tcp_socket.async_read_some(buffer(bytes), read_handler);
      }
    };

  function<void (const boost::system::error_code&) > connectHandler =
    [&](const boost::system::error_code &ec) {
      if (!ec) {
        string r = "GET / HTTP/1.1\r\nHost:theboostcpplibraries.com\r\n\r\n";
        write(tcp_socket, buffer(r));
        tcp_socket.async_read_some(buffer(bytes), read_handler);
      }
    };

  function<void (const boost::system::error_code &, tcp::resolver::iterator)> resolveHandler =
    [&](const boost::system::error_code& ec, tcp::resolver::iterator it) {
      if (!ec)
        tcp_socket.async_connect(*it, connectHandler);
    };

  tcp::resolver::query q{"theboostcpplibraries.com","80"};
  resolver.async_resolve(q, resolveHandler);
  ioservice.run();
}