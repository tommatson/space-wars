#pragma once

#include "../../../engine/imgui/view/view.hpp"
#include "../../../engine/imgui/view/view_manager.hpp"

#include "../../../engine/network/network_manager.hpp"
#include "../../../engine/network/endpoint.hpp"

#include "imgui.h"

#include <optional>


namespace Game::Scenes::MainMenu {

class LobbyView : public Engine::UI::View::View {
public:
  LobbyView(int windowWidth, Engine::UI::View::ViewManager& viewManager, Engine::Network::NetworkManager& networkManager)
      : windowWidth(windowWidth), viewManager(viewManager), networkManager(networkManager) {}

  void render() override {
    const char* title = "LOBBY";
    float titleWidth = ImGui::CalcTextSize(title).x;
    ImGui::SetCursorPosX((windowWidth - titleWidth) * 0.5f);
    ImGui::SetCursorPosY(40.0f);

    ImGui::TextUnformatted(title);

    ImGui::Spacing();

    const char* networkRoleText;

    const char* socketIpText;
    const char* socketPortText;


    if (networkManager.getRole() == Engine::Network::NetworkManager::NetworkRole::Server){
      networkRoleText = "SERVER";
    } else if (networkManager.getRole() == Engine::Network::NetworkManager::NetworkRole::Client){
      networkRoleText = "CLIENT";
    }

    std::optional<Engine::Network::Endpoint> myAddress = networkManager.getTcpSocketAddress();

    socketIpText = myAddress ? myAddress->ip : "ERROR";

    

  }

private:
  int windowWidth;
  Engine::UI::View::ViewManager& viewManager;
  Engine::Network::NetworkManager& networkManager;
};

} // namespace Game::Scenes::MainMenu


