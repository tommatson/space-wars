#pragma once

#include <string>

namespace Game::Core {

class Player {
public:

  Player();

  Player (const Player&) = delete;
  Player &operator=(const Player&) = delete;

  id_t getId();

  std::string getName();

  void changePlayerName(std::string newName);

private:
  using id_t = unsigned int;

  id_t id;
  std::string name;

};

} // namespace Game::Entities 

