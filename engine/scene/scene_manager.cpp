#include "scene_manager.hpp"

#include <iostream>
#include <utility>


namespace Engine::Scene {

bool SceneManager::registerScene(std::string sceneName){
  auto [it, inserted] = sceneRegistry.emplace(sceneName);
  return inserted;
}



bool SceneManager::switchCurrentScene(std::string newSceneName) {

  
}

};
