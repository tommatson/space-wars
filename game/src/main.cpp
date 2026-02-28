#include "../../engine/core/application.hpp"
#include "../../engine/network/network_manager.hpp"

#include "../../engine/scene/scene.hpp"
#include "../scenes/main_menu/main_menu.hpp"



#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>

int main(){

  Engine::Network::NetworkManager networkManager;
  Engine::Core::Application app(std::make_unique<Game::Scenes::MainMenu::MainMenu>(networkManager));

  try{
    app.run();
  }
  catch (const std::exception &e){
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;
}
