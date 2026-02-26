#include "game.hpp"

#include "camera.hpp"
#include "keyboard_movement_controller.hpp"
#include "render_system.hpp"
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
  glm::mat4 projectionView{1.0f};
  glm::vec3 lightDirection = glm::normalize(glm::vec3{1.0f, -3.0f, -1.0f});
};

Game::Game(){
  loadGameObjects();
 
}

Game::~Game(){
}




void Game::run() {
  Buffer globalUboBuffer{
    device, 
    sizeof(GlobalUbo), 
    SwapChain::MAX_FRAMES_IN_FLIGHT,
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
    device.properties.limits.minUniformBufferOffsetAlignment
  };
  globalUboBuffer.map();



  RenderSystem renderSystem{device, renderer.getSwapChainRenderPass()};

  Camera camera{};
  camera.setViewTarget(glm::vec3(-1.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 2.5f));

  auto viewerObject = GameObject::createGameObject();

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
    camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);

    if(auto commandBuffer = renderer.beginFrame()){
      int frameIndex = renderer.getFrameIndex();
      FrameInfo frameInfo{
        frameIndex,
        frameTime,
        commandBuffer,
        camera
      };

      // update
      GlobalUbo ubo{};
      ubo.projectionView = camera.getProjection() * camera.getView();
      globalUboBuffer.writeToIndex(&ubo, frameIndex);
      globalUboBuffer.flushIndex(frameIndex);

      // render 
      renderer.beginSwapChainRenderPass(commandBuffer);
      renderSystem.renderGameObjects(frameInfo, gameObjects);
      renderer.endSwapChainRenderPass(commandBuffer);
      renderer.endFrame();
    }
  } 
  vkDeviceWaitIdle(device.device());


}



void Game::loadGameObjects(){

  std::shared_ptr<Model> model = Model::createModelFromFile(device, "../models/flat_vase.obj"); 
  auto gameObj = GameObject::createGameObject();
  gameObj.model = model;
  gameObj.transform.translation = {0.0f, 0.5f, 2.5f};
  gameObj.transform.scale = glm::vec3(3.0f); 
  gameObjects.push_back(std::move(gameObj));
}




