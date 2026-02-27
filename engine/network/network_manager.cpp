#include "network_manager.hpp"


namespace Engine::Network {

bool NetworkManager::initializeSocket(uint16_t port, NetworkRole newRole){

  if (role != NetworkRole::Uninitialized) return false;  
  if (socket.isOpen()) return false;

  // Error in socket opening
  if (!socket.openSocket(port)) return false;

  std::optional<Endpoint> address = socket.getSocketAddress();

  if (!address) {
    socket.closeSocket();
    return false;
  } 
    
  socketAddress = *address;
  role = newRole;
  
  return true;

}


bool NetworkManager::initializeServer(){
  return initializeSocket(Config::SERVER_PORT, NetworkRole::Server);
}

bool NetworkManager::initializeClient(){
  return initializeSocket(0, NetworkRole::Client);
}


} // namespace Engine::Network
