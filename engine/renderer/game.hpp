#pragma once

#include "vulkan/vulkan_core.h"

#include "descriptors.hpp"
#include "window.hpp"
#include "device.hpp"
#include "game_object.hpp"
#include "renderer.hpp"

#include <memory>
#include <vector>

namespace Engine { namespace Renderer {

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
 
  std::unique_ptr<DescriptorPool> globalPool{};
  GameObject::Map gameObjects;
};

} } // namespace Engine::Renderer
