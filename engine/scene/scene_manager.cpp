#include "scene_manager.hpp"

#include <iostream>
#include <utility>


namespace Engine::Scene {

bool SceneManager::registerScene(std::string sceneName, std::unique_ptr<Scene> scenePtr){

  if (sceneName.empty() || !scenePtr) return false;

  auto [it, inserted] =  sceneRegistry.try_emplace(std::move(sceneName), std::move(scenePtr));

  return inserted;
}


std::string SceneManager::getCurrentScene(){
  return currentScene;
}


bool SceneManager::switchCurrentScene(std::string newSceneName) {

  sceneRegistry.erase(currentScene);
  currentScene = newSceneName;




}

};
