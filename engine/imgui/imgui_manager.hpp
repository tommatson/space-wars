#pragma once

#include "../renderer/window.hpp"
#include "../renderer/device.hpp"
#include "../renderer/descriptors.hpp"

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
