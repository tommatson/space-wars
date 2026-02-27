#pragma once

#include "vulkan/vulkan_core.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace Engine { namespace Renderer {

class Window{

public:
  Window(int w, int h, std::string name);
  ~Window();
  
  // Copy constructor
  Window(const Window &other) = delete;

  // Copy assignment
  Window &operator=(const Window &other) = delete;

  

  bool shouldClose();
  
  VkExtent2D getExtent();

  bool wasWindowResized();

  void resetWindowResizedFlag();

  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

  GLFWwindow* getGLFWwindow() const { return window; }

private:


  void initWindow();

  static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
  
  int width;
  int height;

  bool framebufferResized = false;

  std::string windowName;
  GLFWwindow *window;

};

} } // namespace Engine::Renderer
