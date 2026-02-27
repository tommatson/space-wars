#pragma once

#include "../renderer/game_object.hpp"

#include "../renderer/camera.hpp"
#include "../renderer/keyboard_movement_controller.hpp"
#include "../renderer/systems/render_system.hpp"
#include "../renderer/systems/point_light_system.hpp"
#include "../renderer/buffer.hpp"

#include <string_view>

namespace Engine::Scene {

class Scene{
public:
  
  Scene(const std::string_view name) : sceneName(name){}
  virtual ~Scene() = default;

  Scene(const Scene&) = delete;
  Scene& operator=(const Scene&) = delete;

  Scene(Scene&&) = delete;
  Scene& operator=(Scene&&) = delete;

  virtual void onInit() {}

private:

  Renderer::GameObject::Map gameObjects;
  
  std::string sceneName;

};


} // namespace Engine::Scene
