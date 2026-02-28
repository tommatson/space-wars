#include "player.hpp"


namespace Game::Core {

id_t Player::getId(){
  return id;
}

std::string Player::getName(){
  return name;
}

void Player::changePlayerName(std::string newName){
  name = newName;
}



} // namespace Game::Entities 

