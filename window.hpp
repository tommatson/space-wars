#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>



class Window{

public:
  Window(int w, int h, std::string name);
  ~Window();
  
  // Copy constructor
  Window(const Window &other) = delete;

  // Copy assignment
  Window &operator=(const Window &other) = delete;

  

  bool shouldClose();

  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:


  void initWindow();
  
  const int width;
  const int height;

  std::string windowName;
  GLFWwindow *window;

};

