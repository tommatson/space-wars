#pragma once

#include "scene.hpp"
#include <unordered_set>
#include <memory>

namespace Engine::Scene{

class SceneManager {
public:
  
  struct CurrentScene {
    std::string sceneName;
    std::unique_ptr<Scene> scenePtr;
  };



  SceneManager();
  ~SceneManager();

  SceneManager(const SceneManager&) = delete;
  SceneManager& operator=(const SceneManager&) = delete;

  SceneManager(SceneManager&&) = delete;
  SceneManager& operator=(SceneManager&&) = delete;

  bool registerScene(std::string sceneName);


  bool switchCurrentScene(std::string newSceneName);


  

private:
  


  CurrentScene currentScene;


  std::unordered_set<std::string> sceneRegistry;

};

} // namespace Engine::Scene
