#include "game.hpp"


void Game::run() {

  while(!window.shouldClose()){
    glfwPollEvents();
  }  


}
