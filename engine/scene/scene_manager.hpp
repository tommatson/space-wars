#pragma once

#include "scene.hpp"
#include <unordered_map>
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

  bool registerScene(std::string sceneName, std::unique_ptr<Scene> scenePtr);

  std::string getCurrentScene();

  bool switchCurrentScene(std::string newSceneName);


  

private:
  std::unordered_map<std::string, std::unique_ptr<Scene>> sceneRegistry;

  std::string currentScene;






};

} // namespace Engine::Scene
