#pragma once

#include "window.hpp"
#include "pipeline.hpp"

class Game {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();



private:

  Window window{WIDTH, HEIGHT, "Space Wars"};
  Pipeline pipeline{"../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv"};

};
