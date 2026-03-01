#pragma once
#include "engine/config.hpp"

#include "sockets/socket.hpp"
#include "sockets/udp_socket.hpp"
#include "sockets/tcp_socket.hpp"
#include "endpoint.hpp"

#include <optional>
#include <string>
#include <array>


namespace Engine::Network {


class NetworkManager{
public:

  enum class NetworkRole{
    Uninitialized,
    Server,
    Client
  };
  
  NetworkManager();
  ~NetworkManager();

  NetworkManager (const NetworkManager& ) = delete;
  NetworkManager &operator=(const NetworkManager&) = delete;

  // std::optional<Endpoint> getSocketAddress();

  bool sendMailbox();

  bool initializeServer();
  bool initializeClient();

  std::optional<Endpoint> getUdpSocketAddress();
  std::optional<Endpoint> getTcpSocketAddress();

  NetworkRole getRole();

  std::string convertIpToString(std::array<uint8_t , 16> ip); 

private:


  NetworkRole role = NetworkRole::Uninitialized;

  bool initializeNetwork(uint16_t port, NetworkRole newRole);
  
  bool initializeSocket(uint16_t port, Socket& socket);

  std::optional<Endpoint> getSocketAddress(Socket& socket);

  UdpSocket udpSocket;
  TcpSocket tcpSocket;
};
}

