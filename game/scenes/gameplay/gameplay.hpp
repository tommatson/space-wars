#pragma once

#include "../../../engine/scene/scene.hpp"


namespace Game::Scenes {

class Gameplay final : public Engine::Scene::Scene {
public:
  Gameplay() = default;


  void load(Engine::Renderer::Device& device) override {
    std::shared_ptr<Engine::Renderer::Model> model;

    model = Engine::Renderer::Model::createModelFromFile(device, "../models/flat_vase.obj"); 
    auto gameObj = Engine::Renderer::GameObject::createGameObject();
    gameObj.model = model;
    gameObj.transform.translation = {0.0f, 0.5f, 0.0f};
    gameObj.transform.scale = glm::vec3(3.0f); 
    gameObjects.emplace(gameObj.getId(), std::move(gameObj));

    model = Engine::Renderer::Model::createModelFromFile(device, "../models/quad.obj"); 
    auto floor = Engine::Renderer::GameObject::createGameObject();
    floor.model = model;
    floor.transform.translation = {0.0f, 0.5f, 0.0f};
    floor.transform.scale = {3.0f, 1.0f, 3.0f}; 
    gameObjects.emplace(floor.getId(), std::move(floor)); 



     std::vector<glm::vec3> lightColors{
      {1.f, .1f, .1f},
      {.1f, .1f, 1.f},
      {.1f, 1.f, .1f},
      {1.f, 1.f, .1f},
      {.1f, 1.f, 1.f},
      {1.f, 1.f, 1.f}  
    };

    for (int i = 0; i < lightColors.size(); i++){

      auto pointLight = Engine::Renderer::GameObject::makePointLight(0.2f);
      pointLight.color = lightColors[i];
      auto rotateLight = glm::rotate(glm::mat4(1.0f), 
                                     (i * glm::two_pi<float>()) / lightColors.size(),
                                     {0.0f, -1.0f, 0.0f}
                                     );
      pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
      gameObjects.emplace(pointLight.getId(), std::move(pointLight));

    }

  }

};

} // namespace Game::Scenes
