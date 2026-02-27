#include "../../engine/renderer/game.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>



int main(){
  Game game;

  try{
    game.run();
  }
  catch (const std::exception &e){
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;
}
