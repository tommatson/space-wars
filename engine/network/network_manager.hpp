#pragma once
#include "engine/config.hpp"

#include "sockets/socket.hpp"
#include "endpoint.hpp"

#include <optional>


namespace Engine::Network {


class NetworkManager{
public:
  NetworkManager();
  ~NetworkManager();

  NetworkManager (const NetworkManager& ) = delete;
  NetworkManager &operator=(const NetworkManager&) = delete;

  // std::optional<Endpoint> getSocketAddress();

  bool sendMailbox();

  bool initializeServer();
  bool initializeClient();

private:

  enum class NetworkRole{
    Uninitialized,
    Server,
    Client
  };

  NetworkRole role = NetworkRole::Uninitialized;

  bool initializeNetwork(uint16_t port, NetworkRole newRole);
  
  bool initializeSocket(uint16_t port, Socket socket);

  Socket udpSocket;
  Socket tcpSocket;
};
}

