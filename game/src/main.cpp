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
    std::cout << "Started socket at: " << 
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
