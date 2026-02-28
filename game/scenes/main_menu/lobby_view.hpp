#pragma once

#include "../../../engine/imgui/view/view.hpp"
#include "../../../engine/imgui/view/view_manager.hpp"



namespace Game::Scenes::MainMenu {

class LobbyView : public Engine::UI::View::View {
public:
  LobbyView(int windowWidth, Engine::UI::View::ViewManager& viewManager)
      : windowWidth(windowWidth), viewManager(viewManager) {}

  void render() override {

  }

private:
  int windowWidth;
  Engine::UI::View::ViewManager& viewManager;
};

} // namespace Game::Scenes::MainMenu


