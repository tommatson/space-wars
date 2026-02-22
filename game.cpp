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
 
  createPipeline();

}

Game::~Game(){
  vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}




void Game::run() {

  while(!window.shouldClose()){
    glfwPollEvents();

    if(auto commandBuffer = renderer.beginFrame()){
      renderer.beginSwapChainRenderPass(commandBuffer);
      renderGameObjects(commandBuffer);
      renderer.endSwapChainRenderPass(commandBuffer);
      renderer.endFrame();
    }
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


  assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

  PipelineConfigInfo pipelineConfig{};
  Pipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = renderer.getSwapChainRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  pipeline = std::make_unique<Pipeline>(device, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv", pipelineConfig);
}




void Game::renderGameObjects(VkCommandBuffer commandBuffer){
  pipeline->bind(commandBuffer);

  for (auto& obj : gameObjects){

    obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());

    SimplePushConstantData push{};
    push.offset = obj.transform2d.translation;
    push.color = obj.color;
    push.transform = obj.transform2d.mat2();

    vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);

    obj.model->bind(commandBuffer);
    obj.model->draw(commandBuffer);
  }
}


