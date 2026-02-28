#include "network_manager.hpp"

#include "sockets/socket.hpp"
#include "sockets/udp_socket.hpp"

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

bool NetworkManager::initializeSocket(uint16_t port, Socket& socket){
  if (socket.isOpen()) return false;

  // Error in socket opening
  if (!socket.openSocket(port)) return false;
    
  return true;

}

bool NetworkManager::initializeNetwork(uint16_t port, NetworkRole newRole){
  if (role != NetworkRole::Uninitialized) return false;
  
  udpSocket = UdpSocket(); 
  if (!initializeSocket(port, udpSocket)) return false;

  role = newRole;
  return true;

}


bool NetworkManager::initializeServer(){

  return initializeNetwork(Config::SERVER_PORT, NetworkRole::Server);
}

bool NetworkManager::initializeClient(){
  
  return initializeNetwork(0, NetworkRole::Client);
}

//
// std::optional<Endpoint> NetworkManager::getSocketAddress(){
//
//   std::optional<Endpoint> address = socket.getSocketAddress();
//
//   if (!address) {
//     socket.closeSocket();
//     role = NetworkRole::Uninitialized;
//     return std::nullopt;
//   }
//
//   return address;
//
// }


} // namespace Engine::Network
