#pragma once

#include "../../../engine/imgui/view/view.hpp"
#include "../../../engine/imgui/view/view_manager.hpp"

#include "../../../engine/network/network_manager.hpp"

#include "lobby_view.hpp"


namespace Game::Scenes::MainMenu {

class StartView : public Engine::UI::View::View {
public:
  StartView(int windowWidth, Engine::UI::View::ViewManager& viewManager, Engine::Network::NetworkManager& networkManager)
      : windowWidth(windowWidth), viewManager(viewManager), networkManager(networkManager) {

  }

  void render() override {
    const char* title = "SPACE WARS";
    float titleWidth = ImGui::CalcTextSize(title).x;
    ImGui::SetCursorPosX((windowWidth - titleWidth) * 0.5f);
    ImGui::SetCursorPosY(40.0f);

    ImGui::TextUnformatted(title);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    float buttonWidth = 120.0f;
    float buttonHeight = 40.0f;
    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
    ImGui::SetCursorPosY(100.0f);

    if (ImGui::Button("[ HOST ]", ImVec2(buttonWidth, buttonHeight))) {
        if (networkManager.initializeServer()) {
            viewManager.pushView(std::make_unique<LobbyView>(windowWidth, viewManager, networkManager));
        }
    }

    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
    ImGui::SetCursorPosY(150.0f);

    if (ImGui::Button("[ JOIN ]", ImVec2(buttonWidth, buttonHeight))) {
        if (networkManager.initializeClient()) {
            viewManager.pushView(std::make_unique<LobbyView>(windowWidth, viewManager, networkManager));
        }
    }
  }

private:
  int windowWidth;
  Engine::UI::View::ViewManager& viewManager;
  Engine::Network::NetworkManager& networkManager;
};

} // namespace Game::Scenes::MainMenu

  

