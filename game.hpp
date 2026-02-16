#pragma once

#include "window.hpp"
#include "pipeline.hpp"
#include "device.hpp"

class Game {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();



private:

  Window window{WIDTH, HEIGHT, "Space Wars"};
  Device device{window};  
  Pipeline pipeline{device, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv", Pipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};

};
