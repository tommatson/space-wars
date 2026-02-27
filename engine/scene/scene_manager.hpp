#pragma once

#include "scene.hpp"

#include "../renderer/game_object.hpp"


#include <memory>

namespace Engine::Scene{

class SceneManager {
public:

  SceneManager(std::unique_ptr<Scene> newScene);
  ~SceneManager();

  SceneManager(const SceneManager&) = delete;
  SceneManager& operator=(const SceneManager&) = delete;

  SceneManager(SceneManager&&) = delete;
  SceneManager& operator=(SceneManager&&) = delete;


  void loadScene(Renderer::Device& device);
  void switchCurrentScene(std::unique_ptr<Scene> newScene, Renderer::Device& device);

  Renderer::GameObject::Map& getCurrentSceneGameObjects();



private:

  std::unique_ptr<Scene> currentScene;

};

} // namespace Engine::Scene
