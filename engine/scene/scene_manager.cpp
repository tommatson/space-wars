#include "scene_manager.hpp"


namespace Engine::Scene {

SceneManager::SceneManager(std::unique_ptr<Scene> newScene){
  currentScene = std::move(newScene);
  currentScene->setSceneManager(this);
}

SceneManager::~SceneManager() = default;

void SceneManager::loadScene(Renderer::Device& device) {
  currentScene->load(device);
}

void SceneManager::switchCurrentScene(std::unique_ptr<Scene> newScene, Renderer::Device& device) {
  currentScene = std::move(newScene);
  currentScene->setSceneManager(this);
  currentScene->load(device);
}

void SceneManager::requestSceneSwitch(std::unique_ptr<Scene> newScene) {
  pendingScene = std::move(newScene);
}

void SceneManager::processPendingSceneSwitch(Renderer::Device& device) {
  if (pendingScene) {
    switchCurrentScene(std::move(pendingScene), device);
  }
}

Renderer::GameObject::Map& SceneManager::getCurrentSceneGameObjects() {
  return currentScene->getGameObjects();
}


};

