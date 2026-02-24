#pragma once

#include "vulkan/vulkan_core.h"
#include "pipeline.hpp"
#include "device.hpp"
#include "game_object.hpp"

#include <memory>
#include <vector>

class RenderSystem {
public:


  RenderSystem(Device& device, VkRenderPass renderPass);
  ~RenderSystem();

  RenderSystem(const RenderSystem &) = delete;
  RenderSystem &operator=(const RenderSystem &) = delete;


  void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects);



private:
  void createPipelineLayout();
  void createPipeline(VkRenderPass renderPass);

  Device& device;  

  std::unique_ptr<Pipeline> pipeline;
  VkPipelineLayout pipelineLayout;
 
};
