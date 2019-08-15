//
// Created by xxzyjy on 31/07/2017.
//
#include <boost/asio/io_service.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <list>
#include <string>
#include <ctime>
#include <functional>
#include <gtest/gtest.h>

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace std;
using namespace std::placeholders;

void doWrite(tcp::socket &tcp_socket, yield_context yield) {
  time_t now = time(nullptr);
  string data = ctime(&now);
  async_write(tcp_socket, buffer(data),yield);
  tcp_socket.shutdown(tcp::socket::shutdown_send);
}

void doAccept(io_service& ioservice, list<tcp::socket>& tcpSockets, tcp::acceptor& tcpAcceptor, yield_context yield) {
  for (int i = 0;i < 2; ++i){
    tcpSockets.emplace_back(ioservice);
    tcpAcceptor.async_accept(tcpSockets.back(), yield);
    spawn(ioservice,[&](yield_context yield){
      doWrite(tcpSockets.back(),yield);
    });
  }
}

TEST(Coroutines,demo1){
  io_service ioservice;
  tcp::endpoint tcpEndpoint{tcp::v4(),2014};
  tcp::acceptor tcpAcceptor{ioservice,tcpEndpoint};
  list<tcp::socket> tcpSockets;

  tcpAcceptor.listen();
  spawn(ioservice,bind(doAccept, ref(ioservice), ref(tcpSockets),ref(tcpAcceptor),_1));
  ioservice.run();
}

