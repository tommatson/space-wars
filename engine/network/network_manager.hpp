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

  bool initializeServer();
  bool initializeClient();

  std::optional<Endpoint> getSocketAddress();

  bool sendMailbox();


private:

  enum class NetworkRole{
    Uninitialized,
    Client,
    Server
  };

  NetworkRole role = NetworkRole::Uninitialized;

  Socket socket;

  bool initializeSocket(uint16_t port, NetworkRole newRole);





};
}

