#pragma once


#include "../renderer/window.hpp"
#include "../renderer/device.hpp"
#include "../renderer/renderer.hpp"
#include "../renderer/descriptors.hpp"
#include "../renderer/game_object.hpp"

#include <memory>
#include <vector>

namespace Engine::Core {

class Application {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;


  Application();
  ~Application();

  Application(const Application &) = delete;
  Application &operator=(const Application &) = delete;


  void run();



private:
  void loadGameObjects();

  Renderer::Window window{WIDTH, HEIGHT, "Space Wars"};
  Renderer::Device device{window};  

  Renderer::Renderer renderer{window, device};
 
  std::unique_ptr<Renderer::DescriptorPool> globalPool{};
  Renderer::GameObject::Map gameObjects;
};

} // namespace Engine::Core
