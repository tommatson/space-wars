#pragma once

#include "vulkan/vulkan_core.h"
#include "window.hpp"
#include "device.hpp"
#include "game_object.hpp"
#include "renderer.hpp"

#include <memory>
#include <vector>

class Game {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;


  Game();
  ~Game();

  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;


  void run();



private:
  void loadGameObjects();

  Window window{WIDTH, HEIGHT, "Space Wars"};
  Device device{window};  

  Renderer renderer{window, device};
 
  std::vector<GameObject> gameObjects;
};
