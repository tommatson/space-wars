#pragma once

#include "../../engine/scene/scene.hpp"
#include "../../engine/scene/scene_manager.hpp"
#include "gameplay.hpp"

#include "imgui.h"

#include <memory>

namespace Game::Scenes {

class MainMenu final : public Engine::Scene::Scene {
public:
  MainMenu() = default;

  void load(Engine::Renderer::Device& device) override {
    devicePtr = &device;
  }

  void renderUI() override {
    // Get the display size for centering
    ImGuiIO& io = ImGui::GetIO();
    float windowWidth = 300.0f;
    float windowHeight = 200.0f;

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

    // Title
    const char* title = "SPACE WARS";
    float titleWidth = ImGui::CalcTextSize(title).x;
    ImGui::SetCursorPosX((windowWidth - titleWidth) * 0.5f);
    ImGui::SetCursorPosY(40.0f);

    ImGui::PushFont(nullptr); // Use default font
    ImGui::TextUnformatted(title);
    ImGui::PopFont();

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    // Play button — centered
    float buttonWidth = 120.0f;
    float buttonHeight = 40.0f;
    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
    ImGui::SetCursorPosY(100.0f);

    if (ImGui::Button("Play", ImVec2(buttonWidth, buttonHeight))) {
      if (sceneManagerPtr && devicePtr) {
        sceneManagerPtr->requestSceneSwitch(std::make_unique<Gameplay>());
      }
    }

    ImGui::End();
  }

private:
  Engine::Renderer::Device* devicePtr = nullptr;
};

} // namespace Game::Scenes
