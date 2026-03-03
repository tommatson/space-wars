#pragma once



#include <asio.hpp>
#include <memory>
#include <vector>
#include <iostream>

namespace Engine::Network {

class Connection {
public:

  Connection(uint32_t id, asio::ip::tcp::socket socket) : id_(id), socket_(std::move(socket)) {}


  void start() {
    read();
  }


private:


  void read(){


  }



  uint32_t id_;
  asio::ip::tcp::socket socket_;
};

} // namespace Engine::Network
