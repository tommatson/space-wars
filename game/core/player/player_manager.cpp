#include "player_manager.hpp"


namespace Game::Core {


void PlayerManager::addPlayer(std::unique_ptr<Player> player){
  PlayerManager::id_t id = player->getId();
  playerMap.emplace(id, std::move(player));
}


} // namespace Game::Entities 




