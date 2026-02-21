#pragma once

#include "vulkan/vulkan_core.h"
#include "window.hpp"
#include "pipeline.hpp"
#include "device.hpp"
#include "swap_chain.hpp"
#include "model.hpp"

#include <memory>
#include <vector>

class Game {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;


  Game();
  ~Game();

  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;


  void run();



private:
  void loadModels();
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void freeCommandBuffers();
  void drawFrame();
  void recreateSwapChain();
  void recordCommandBuffer(int imageIndex);

  Window window{WIDTH, HEIGHT, "Space Wars"};
  Device device{window};  
  std::unique_ptr<SwapChain> swapChain;
  std::unique_ptr<Pipeline> pipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;
  std::unique_ptr<Model> model;
};
