#pragma once

#include "scene.hpp"
#include <unordered_set>
#include <memory>

namespace Engine::Scene{

class SceneManager {
public:

  SceneManager();
  ~SceneManager();

  SceneManager(const SceneManager&) = delete;
  SceneManager& operator=(const SceneManager&) = delete;

  SceneManager(SceneManager&&) = delete;
  SceneManager& operator=(SceneManager&&) = delete;


  void switchCurrentScene(std::unique_ptr<Scene> newScene);


  

private:

  std::unique_ptr<Scene> currentScene;

};

} // namespace Engine::Scene
