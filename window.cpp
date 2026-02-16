#include "window.hpp"
#include <string>


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
