#include "game.hpp"

#include "render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


#include <memory>
#include <stdexcept>
#include <array>
#include <cassert>


Game::Game(){
  loadGameObjects();
 
}

Game::~Game(){
}




void Game::run() {
  RenderSystem renderSystem{device, renderer.getSwapChainRenderPass()};

  while(!window.shouldClose()){
    glfwPollEvents();

    if(auto commandBuffer = renderer.beginFrame()){
      renderer.beginSwapChainRenderPass(commandBuffer);
      renderSystem.renderGameObjects(commandBuffer, gameObjects);
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




