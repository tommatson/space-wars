#pragma once
#include <asio.hpp>
#include <iostream>

namespace Engine::Network {

class TcpServer {
public:

  TcpServer();


  void start(){
    std::cout << "Engine network starting..." << '\n';

    accept();
  }






private:

  void accept() {


    acceptor_.async_accept(
      [this] (std::error_code ec, asio::ip::tcp::socket socket) {
        if (!ec) {
        }
      }
    );

  }

  asio::ip::tcp::acceptor acceptor_;


};

} // namespace engine::net
