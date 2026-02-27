#pragma once


#include "../renderer/window.hpp"
#include "../renderer/device.hpp"
#include "../renderer/renderer.hpp"
#include "../renderer/descriptors.hpp"
#include "../renderer/game_object.hpp"
#include "../imgui/imgui_manager.hpp"

#include "../scene/scene_manager.hpp"
#include "../scene/scene.hpp"

#include <memory>
#include <vector>

namespace Engine::Core {

class Application {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;


  Application(std::unique_ptr<Scene::Scene> initialScene);
  ~Application();

  Application(const Application &) = delete;
  Application &operator=(const Application &) = delete;


  void run();

  Renderer::Device& getDevice() { return device; }



private:
  void loadGameObjects();

  Renderer::Window window{WIDTH, HEIGHT, "Space Wars"};
  Renderer::Device device{window};  

  Renderer::Renderer renderer{window, device};
 
  std::unique_ptr<Renderer::DescriptorPool> globalPool{};

  ImGuiManager imguiManager;
  Scene::SceneManager sceneManager;
};

} // namespace Engine::Core
