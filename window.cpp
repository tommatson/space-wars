#include "window.hpp"
#include "vulkan/vulkan_core.h"

#include <stdexcept>
#include <string>
#include <stdexcept>


Window::Window(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
  initWindow();
}
Window::~Window(){
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Window::initWindow(){
  glfwInit();
  // Disable API creation as using Vulkan
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);


}

bool Window::shouldClose(){
  return glfwWindowShouldClose(window);
}

VkExtent2D Window::getExtent() {
  return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
}


void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS){
    throw std::runtime_error("Failed to create window surface.");
  }
}
