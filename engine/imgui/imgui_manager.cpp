#include "imgui_manager.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include <stdexcept>

namespace Engine {

void ImGuiManager::init(Renderer::Window& window, Renderer::Device& device, VkRenderPass renderPass, uint32_t imageCount) {
  // Create a dedicated descriptor pool for ImGui
  VkDescriptorPoolSize poolSizes[] = {
    { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
  };

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
  poolInfo.maxSets = 1;
  poolInfo.poolSizeCount = 1;
  poolInfo.pPoolSizes = poolSizes;

  VkDescriptorPool imguiPool;
  if (vkCreateDescriptorPool(device.device(), &poolInfo, nullptr, &imguiPool) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create ImGui descriptor pool");
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  // Setup style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForVulkan(window.getGLFWwindow(), true);

  ImGui_ImplVulkan_InitInfo initInfo{};
  initInfo.Instance = device.getInstance();
  initInfo.PhysicalDevice = device.getPhysicalDevice();
  initInfo.Device = device.device();

  auto queueFamilyIndices = device.findPhysicalQueueFamilies();
  initInfo.QueueFamily = queueFamilyIndices.graphicsFamily;
  initInfo.Queue = device.graphicsQueue();
  initInfo.DescriptorPool = imguiPool;
  initInfo.MinImageCount = 2;
  initInfo.ImageCount = imageCount;
  initInfo.PipelineInfoMain.RenderPass = renderPass;
  initInfo.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

  ImGui_ImplVulkan_Init(&initInfo);

  initialized = true;
}

void ImGuiManager::newFrame() {
  if (!initialized) return;
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiManager::render(VkCommandBuffer commandBuffer) {
  if (!initialized) return;
  ImGui::Render();
  ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
}

void ImGuiManager::shutdown() {
  if (!initialized) return;
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  initialized = false;
}

} // namespace Engine
