#pragma once

#include <string>


namespace Engine::Scene {

class Scene{
public:
  
  virtual ~Scene() = default;

  virtual void load();

};


} // namespace Engine::Scene
