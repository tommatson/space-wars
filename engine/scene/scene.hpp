#pragma once

#include "../renderer/game_object.hpp"

#include "../renderer/device.hpp"


#include <string>


namespace Engine::Scene {

class Scene{
public:

  Scene() = default;
  Scene(const Scene&) = delete;
  Scene& operator=(const Scene&) = delete;

  virtual ~Scene() = default;



  virtual void load(Renderer::Device& device) = 0;


  Renderer::GameObject::Map& getGameObjects() { return gameObjects; };
  const Renderer::GameObject::Map& getGameObjects() const { return gameObjects; };

protected:

  Renderer::GameObject::Map gameObjects;

};


} // namespace Engine::Scene
