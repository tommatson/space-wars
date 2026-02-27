#pragma once

#include "../../engine/scene/scene.hpp"
#include <string_view>

namespace Game::Scenes {

class Gameplay final : public Engine::Scene::Scene {
public:
  explicit Gameplay() : Engine::Scene::Scene("Gameplay") {}
  ~Gameplay() override = default;

  void onInit() override {

  }


};

} // namespace Game::Scenes
