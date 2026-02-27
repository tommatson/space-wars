#include "application.hpp"

#include "../renderer/camera.hpp"
#include "../renderer/keyboard_movement_controller.hpp"
#include "../renderer/systems/render_system.hpp"
#include "../renderer/systems/point_light_system.hpp"
#include "../renderer/buffer.hpp"



#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


#include <memory>
#include <chrono>
#include <stdexcept>
#include <array>
#include <cassert>

namespace Engine::Core{

Application::Application(std::unique_ptr<Scene::Scene> initialScene) : sceneManager(std::move(initialScene)){
  globalPool = Renderer::DescriptorPool::Builder(device)
    .setMaxSets(Renderer::SwapChain::MAX_FRAMES_IN_FLIGHT)
    .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, Renderer::SwapChain::MAX_FRAMES_IN_FLIGHT)
    .build();

  // Initialize ImGui
  imguiManager.init(window, device, renderer.getSwapChainRenderPass(), Renderer::SwapChain::MAX_FRAMES_IN_FLIGHT);

  loadGameObjects();
}

Application::~Application(){
  imguiManager.shutdown();
}


void Application::run() {

  std::vector<std::unique_ptr<Renderer::Buffer>> uboBuffers(Renderer::SwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < uboBuffers.size(); i++){
    uboBuffers[i]= std::make_unique<Renderer::Buffer>(
      device, 
      sizeof(Renderer::GlobalUbo), 
      1,
      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
    );
    uboBuffers[i]->map();
  }


  auto globalSetLayout = Renderer::DescriptorSetLayout::Builder(device)
    .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
    .build();

  std::vector<VkDescriptorSet> globalDescriptorSets(Renderer::SwapChain::MAX_FRAMES_IN_FLIGHT);
  
  for(int i = 0; i < globalDescriptorSets.size(); i++){
    auto bufferInfo = uboBuffers[i]->descriptorInfo();
    Renderer::DescriptorWriter(*globalSetLayout, *globalPool)
      .writeBuffer(0, &bufferInfo)
      .build(globalDescriptorSets[i]);
  }

  Renderer::RenderSystem renderSystem{device, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};

  Renderer::PointLightSystem pointLightSystem{device, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};


  Renderer::Camera camera{};
  camera.setViewTarget(glm::vec3(-1.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 2.5f));

  auto viewerObject = Renderer::GameObject::createGameObject();
  viewerObject.transform.translation.z = -2.5f;

  Renderer::KeyboardMovementController cameraController{};

  auto currentTime = std::chrono::high_resolution_clock::now();

  while(!window.shouldClose()){
    glfwPollEvents();

    
    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
    currentTime = newTime;

    // frameTime = glm::min(frameTime, MAX_FRAME_TIME);

    // Update current scene
    sceneManager.getCurrentScene()->update(frameTime);

    cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);



    float aspect = renderer.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 100.0f);

    if(auto commandBuffer = renderer.beginFrame()){
      int frameIndex = renderer.getFrameIndex();
      Renderer::FrameInfo frameInfo{
        frameIndex,
        frameTime,
        commandBuffer,
        camera,
        globalDescriptorSets[frameIndex],
        sceneManager.getCurrentSceneGameObjects()
      };

      // update
      Renderer::GlobalUbo ubo{};
      ubo.projection = camera.getProjection(); 
      ubo.view = camera.getView();
      pointLightSystem.update(frameInfo, ubo);
      uboBuffers[frameIndex]->writeToBuffer(&ubo);
      uboBuffers[frameIndex]->flush();
      // render 
      renderer.beginSwapChainRenderPass(commandBuffer);
      renderSystem.renderGameObjects(frameInfo);
      pointLightSystem.render(frameInfo);

      // ImGui render
      imguiManager.newFrame();
      sceneManager.getCurrentScene()->renderUI();
      imguiManager.render(commandBuffer);

      renderer.endSwapChainRenderPass(commandBuffer);
      renderer.endFrame();
    }

    // Process any pending scene switch (after frame is done)
    sceneManager.processPendingSceneSwitch(device);
  } 
  vkDeviceWaitIdle(device.device());


}


void Application::loadGameObjects(){
  sceneManager.loadScene(device);
}



} // namespace Engine::Core
