#include "network_manager.hpp"


namespace Engine::Network {

NetworkManager::NetworkManager() {}
NetworkManager::~NetworkManager() {}

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
