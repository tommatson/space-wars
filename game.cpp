#include "game.hpp"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


#include <memory>
#include <stdexcept>
#include <array>
#include <cassert>


struct SimplePushConstantData {
  glm::mat2 transform{1.0f};
  glm::vec2 offset;
  alignas(16) glm::vec3 color;

};


Game::Game(){
  loadGameObjects();
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

void Game::loadGameObjects(){
  std::vector<Model::Vertex> verticies{
    {{0.0f, -0.5f}, {1.0, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}}
  };

  auto model = std::make_shared<Model>(device, verticies);

  auto triangle = GameObject::createGameObject();

  triangle.model = model;
  triangle.color = {0.1f, 0.8f, 0.1f};
  triangle.transform2d.translation.x = 0.2f;
  triangle.transform2d.scale  = {2.0f, 0.5f};
  triangle.transform2d.rotation = 0.25f * glm::two_pi<float>(); 

  gameObjects.push_back(std::move(triangle));
}

void Game::createPipelineLayout(){

  VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(SimplePushConstantData);




  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

  if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS){
    throw std::runtime_error("Failed to create pipeline layout");
  }


}



void Game::createPipeline(){


  assert(swapChain != nullptr && "Cannot create pipeline before swap chain");
  assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

  PipelineConfigInfo pipelineConfig{};
  Pipeline::defaultPipelineConfigInfo(pipelineConfig);
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

  if (swapChain == nullptr){
    swapChain = std::make_unique<SwapChain>(device, extent);
  } else {
    swapChain = std::make_unique<SwapChain>(device, extent, std::move(swapChain));    
    if (swapChain->imageCount() != commandBuffers.size()){
      freeCommandBuffers();
      createCommandBuffers();
    }
  }
  
  // Future optimisation is check if render passes are compatable
  // If yes, then render pipeline does not need to be created
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


void Game::freeCommandBuffers(){
  vkFreeCommandBuffers(device.device(), device.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
  commandBuffers.clear();
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


  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
  viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{{0, 0}, swapChain->getSwapChainExtent()};
  vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
  vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

  renderGameObjects(commandBuffers[imageIndex]);

  vkCmdEndRenderPass(commandBuffers[imageIndex]);

  if(vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS){
    throw std::runtime_error("Failed to record command buffer");
  }
}

void Game::renderGameObjects(VkCommandBuffer commandBuffer){
  pipeline->bind(commandBuffer);

  for (auto& obj : gameObjects){
    SimplePushConstantData push{};
    push.offset = obj.transform2d.translation;
    push.color = obj.color;
    push.transform = obj.transform2d.mat2();

    vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);

    obj.model->bind(commandBuffer);
    obj.model->draw(commandBuffer);
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
