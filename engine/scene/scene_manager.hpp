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

  // Deferred scene switch — queues the switch to be processed between frames
  void requestSceneSwitch(std::unique_ptr<Scene> newScene);
  void processPendingSceneSwitch(Renderer::Device& device);

  Renderer::GameObject::Map& getCurrentSceneGameObjects();

  Scene* getCurrentScene() { return currentScene.get(); }


private:

  std::unique_ptr<Scene> currentScene;
  std::unique_ptr<Scene> pendingScene;

};

} // namespace Engine::Scene

