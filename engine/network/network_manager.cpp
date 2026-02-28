#include "network_manager.hpp"

#if defined(_WIN32)
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
typedef int socklen_t;
#endif



namespace Engine::Network {

NetworkManager::NetworkManager() {
  
#if defined(_WIN32)
  WSADATA WsaData;
  WSAStartup(MAKEWORD(2, 2), &WsaData);
#endif

}

NetworkManager::~NetworkManager() {
#if defined(_WIN32)
  WSACleanup();
#endif
}

bool NetworkManager::initializeSocket(uint16_t port, NetworkRole newRole){


  if (role != NetworkRole::Uninitialized) return false;  
  if (socket.isOpen()) return false;

  // Error in socket opening
  if (!socket.openSocket(port)) return false;
    
  role = newRole;
  
  return true;

}


bool NetworkManager::initializeServer(){
  return initializeSocket(Config::SERVER_PORT, NetworkRole::Server);
}

bool NetworkManager::initializeClient(){
  return initializeSocket(0, NetworkRole::Client);
}


std::optional<Endpoint> NetworkManager::getSocketAddress(){

  std::optional<Endpoint> address = socket.getSocketAddress();

  if (!address) {
    socket.closeSocket();
    role = NetworkRole::Uninitialized;
    return std::nullopt;
  }

  return address;

}


} // namespace Engine::Network
