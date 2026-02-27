#include "scene_manager.hpp"


namespace Engine::Scene {

SceneManager::SceneManager(std::unique_ptr<Scene> newScene){
  currentScene = std::move(newScene);
}

SceneManager::~SceneManager() = default;

void SceneManager::loadScene(Renderer::Device& device) {
  currentScene->load(device);
}

void SceneManager::switchCurrentScene(std::unique_ptr<Scene> newScene, Renderer::Device& device) {
  currentScene = std::move(newScene);
  currentScene->load(device);
}


Renderer::GameObject::Map& SceneManager::getCurrentSceneGameObjects() {
  return currentScene->getGameObjects();
}


};
