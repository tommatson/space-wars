#pragma once

#include "player.hpp"

#include <cstdint>
#include <unordered_map>
#include <memory>


namespace Game::Core {

class PlayerManager{
public:

  PlayerManager();

  PlayerManager (const PlayerManager&) = delete;
  PlayerManager &operator=(const PlayerManager&) = delete;

  void addPlayer(std::unique_ptr<Player> player);

private:

  using id_t = uint8_t; 

  enum class PlayerNetworkStatus {
    Connected,
    Disconnected
  };

  std::unordered_map<id_t, std::unique_ptr<Player>> playerMap;

};

} // namespace Game::Entities 


