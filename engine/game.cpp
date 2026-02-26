#include "game.hpp"

#include "camera.hpp"
#include "keyboard_movement_controller.hpp"
#include "systems/render_system.hpp"
#include "systems/point_light_system.hpp"
#include "buffer.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


#include <memory>
#include <chrono>
#include <stdexcept>
#include <array>
#include <cassert>

struct GlobalUbo {
  glm::mat4 projection{1.0f};
  glm::mat4 view{1.0f};
  glm::vec4 ambientLightColor{1.0f, 1.0f, 1.0f, 0.02f};
  glm::vec3 lightPosition{-1.0f};
  alignas(16) glm::vec4 lightColor{1.0f};
};


Game::Game(){
  globalPool = DescriptorPool::Builder(device)
    .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
    .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
    .build();
  loadGameObjects();
 
}

Game::~Game(){
}


void Game::run() {

  std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < uboBuffers.size(); i++){
    uboBuffers[i]= std::make_unique<Buffer>(
      device, 
      sizeof(GlobalUbo), 
      1,
      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
    );
    uboBuffers[i]->map();
  }


  auto globalSetLayout = DescriptorSetLayout::Builder(device)
    .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
    .build();

  std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
  
  for(int i = 0; i < globalDescriptorSets.size(); i++){
    auto bufferInfo = uboBuffers[i]->descriptorInfo();
    DescriptorWriter(*globalSetLayout, *globalPool)
      .writeBuffer(0, &bufferInfo)
      .build(globalDescriptorSets[i]);
  }

  RenderSystem renderSystem{device, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};

  PointLightSystem pointLightSystem{device, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};


  Camera camera{};
  camera.setViewTarget(glm::vec3(-1.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 2.5f));

  auto viewerObject = GameObject::createGameObject();
  viewerObject.transform.translation.z = -2.5f;

  KeyboardMovementController cameraController{};

  auto currentTime = std::chrono::high_resolution_clock::now();

  while(!window.shouldClose()){
    glfwPollEvents();

    
    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
    currentTime = newTime;

    // frameTime = glm::min(frameTime, MAX_FRAME_TIME);

    cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);



    float aspect = renderer.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 100.0f);

    if(auto commandBuffer = renderer.beginFrame()){
      int frameIndex = renderer.getFrameIndex();
      FrameInfo frameInfo{
        frameIndex,
        frameTime,
        commandBuffer,
        camera,
        globalDescriptorSets[frameIndex],
        gameObjects
      };

      // update
      GlobalUbo ubo{};
      ubo.projection = camera.getProjection(); 
      ubo.view = camera.getView();

      uboBuffers[frameIndex]->writeToBuffer(&ubo);
      uboBuffers[frameIndex]->flush();
      // render 
      renderer.beginSwapChainRenderPass(commandBuffer);
      renderSystem.renderGameObjects(frameInfo);
      pointLightSystem.render(frameInfo);
      renderer.endSwapChainRenderPass(commandBuffer);
      renderer.endFrame();
    }
  } 
  vkDeviceWaitIdle(device.device());


}



void Game::loadGameObjects(){

  std::shared_ptr<Model> model;

  model = Model::createModelFromFile(device, "../models/flat_vase.obj"); 
  auto gameObj = GameObject::createGameObject();
  gameObj.model = model;
  gameObj.transform.translation = {0.0f, 0.5f, 0.0f};
  gameObj.transform.scale = glm::vec3(3.0f); 
  gameObjects.emplace(gameObj.getId(), std::move(gameObj));

  model = Model::createModelFromFile(device, "../models/quad.obj"); 
  auto floor = GameObject::createGameObject();
  floor.model = model;
  floor.transform.translation = {0.0f, 0.5f, 0.0f};
  floor.transform.scale = {3.0f, 1.0f, 3.0f}; 
  gameObjects.emplace(floor.getId(), std::move(floor)); 

  

}




