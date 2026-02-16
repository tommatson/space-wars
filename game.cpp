#include "game.hpp"
#include "vulkan/vulkan_core.h"

#include <stdexcept>


Game::Game(){
  createPipelineLayout();
  createPipeline();
  createCommandBuffers();

}

Game::~Game(){
  vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}




void Game::run() {

  while(!window.shouldClose()){
    glfwPollEvents();
  }  


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
  auto pipelineConfig = Pipeline::defaultPipelineConfigInfo(swapChain.width(), swapChain.height());
  pipelineConfig.renderPass = swapChain.getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  pipeline = std::make_unique<Pipeline>(device, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv", pipelineConfig);
}



void Game::createCommandBuffers(){}

void Game::drawFrame(){}
