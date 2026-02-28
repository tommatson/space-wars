#pragma once

#include "../../../engine/scene/scene.hpp"
#include "../../../engine/scene/scene_manager.hpp"
#include "../gameplay/gameplay.hpp"

#include "../../../engine/imgui/view/view_manager.hpp"
#include "../../../engine/imgui/view/view.hpp"

#include "start_view.hpp"

#include "imgui.h"

#include <memory>

namespace Game::Scenes::MainMenu {

class MainMenu final : public Engine::Scene::Scene {
public:
  MainMenu() 
      : windowWidth(300.0f), 
        windowHeight(200.0f),
        viewManager(nullptr) 
  {
    viewManager.pushView(std::make_unique<Game::Scenes::MainMenu::StartView>(windowWidth, viewManager));
  }

  void load(Engine::Renderer::Device& device) override {
    devicePtr = &device;
  }

  void renderUI() override {

    // Get the display size for centering
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowPos(
      ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
      ImGuiCond_Always,
      ImVec2(0.5f, 0.5f)
    );
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

    ImGui::Begin("##MainMenu", nullptr,
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoBackground
    );

    viewManager.render();

    ImGui::End();
  }

private:
  Engine::Renderer::Device* devicePtr = nullptr;
  
  float windowWidth;
  float windowHeight;

  Engine::UI::View::ViewManager viewManager; 
};

} // namespace Game::Scenes::MainMenu
