#pragma once

#include "../../engine/renderer/window.hpp"
#include "../../engine/renderer/device.hpp"
#include "../../engine/renderer/descriptors.hpp"

#include <vulkan/vulkan.h>

namespace Engine {

class ImGuiManager {
public:
  ImGuiManager() = default;
  ~ImGuiManager() = default;

  ImGuiManager(const ImGuiManager&) = delete;
  ImGuiManager& operator=(const ImGuiManager&) = delete;

  void init(Renderer::Window& window, Renderer::Device& device, VkRenderPass renderPass, uint32_t imageCount);
  void newFrame();
  void render(VkCommandBuffer commandBuffer);
  void shutdown();

private:
  bool initialized = false;
};

} // namespace Engine
