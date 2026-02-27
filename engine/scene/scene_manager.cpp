#include "scene_manager.hpp"


namespace Engine::Scene {

void SceneManager::switchCurrentScene(std::unique_ptr<Scene> newScene) {
  currentScene = std::move(newScene);
  currentScene->load();
}

};
