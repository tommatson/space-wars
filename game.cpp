#include "game.hpp"
#include "swap_chain.hpp"
#include "vulkan/vulkan_core.h"

#include <memory>
#include <stdexcept>
#include <array>


Game::Game(){
  loadModels();
  createPipelineLayout();
  recreateSwapChain();
  createCommandBuffers();

}

Game::~Game(){
  vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}




void Game::run() {

  while(!window.shouldClose()){
    glfwPollEvents();
    drawFrame();
  } 
  vkDeviceWaitIdle(device.device());


}

void Game::loadModels(){
  std::vector<Model::Vertex> verticies{
    {{0.0f, -0.5f}, {1.0, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}}
  };

  model = std::make_unique<Model>(device, verticies);
}

void Game::createPipelineLayout(){
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;

  if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS){
    throw std::runtime_error("Failed to create pipeline layout");
  }


}



void Game::createPipeline(){
  auto pipelineConfig = Pipeline::defaultPipelineConfigInfo(swapChain->width(), swapChain->height());
  pipelineConfig.renderPass = swapChain->getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  pipeline = std::make_unique<Pipeline>(device, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv", pipelineConfig);
}


void Game::recreateSwapChain() {
  auto extent = window.getExtent();
  while (extent.width == 0 || extent.height == 0){
    extent = window.getExtent();
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(device.device());
  swapChain = std::make_unique<SwapChain>(device, extent);
  createPipeline();

}


void Game::createCommandBuffers(){
  commandBuffers.resize(swapChain->imageCount());

  VkCommandBufferAllocateInfo allocInfo{};

  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = device.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS){
    throw std::runtime_error("Failed to allocate command buffers!");
  }

}


void Game::recordCommandBuffer(int imageIndex){
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if(vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS){
    throw std::runtime_error("Failed to begin recording command buffer");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = swapChain->getRenderPass();
  renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

  pipeline->bind(commandBuffers[imageIndex]);
  model->bind(commandBuffers[imageIndex]);
  model->draw(commandBuffers[imageIndex]);


  vkCmdEndRenderPass(commandBuffers[imageIndex]);

  if(vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS){
    throw std::runtime_error("Failed to record command buffer");
  }
}

void Game::drawFrame(){

  uint32_t imageIndex;
  auto result = swapChain->acquireNextImage(&imageIndex);
  

  if (result == VK_ERROR_OUT_OF_DATE_KHR){
    recreateSwapChain();
    return;
  }
  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
    throw std::runtime_error("Failed to acquire swapchain image");
  }

  recordCommandBuffer(imageIndex);
  result = swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR ||  result == VK_SUBOPTIMAL_KHR || window.wasWindowResized()){
    window.resetWindowResizedFlag();
    recreateSwapChain();
    return;
  }


  if (result != VK_SUCCESS){
    throw std::runtime_error("Failed to present swap chain image");
  }


}
