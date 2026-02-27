#include "../../engine/renderer/game.hpp"
#include "../../engine/network/network_manager.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>

int main(){
  Engine::Renderer::Game game;
  Engine::Network::NetworkManager networkManager;

  
  if(networkManager.initializeClient()){
    std::optional<Engine::Network::Endpoint> socketAddress = networkManager.getSocketAddress();
    if (socketAddress){
      Engine::Network::Endpoint address = *socketAddress;
      std::cout << "Socket opened on port " << address.port << '\n';
    } else {
      std::cout << "Failed to get socket address" << '\n';
    }
  } else{
    std::cout << "Failed to initialize client" << '\n';
  }
  
  

  try{
    game.run();
  }
  catch (const std::exception &e){
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;
}
