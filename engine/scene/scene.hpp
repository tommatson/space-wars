#pragma once

#include <string>


namespace Engine::Scene {

class Scene{
public:
  
  virtual ~Scene() = default;

  virtual std::unique_ptr<Scene> load();

};


} // namespace Engine::Scene
