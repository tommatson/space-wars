#include "../../engine/core/application.hpp"
#include "../../engine/network/network_manager.hpp"

#include "../../engine/scene/scene.hpp"
#include "../scenes/main_menu.hpp"



#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>

int main(){

  Engine::Core::Application app(std::make_unique<Game::Scenes::MainMenu>());
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
    app.run();
  }
  catch (const std::exception &e){
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;
}
