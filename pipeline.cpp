#include "pipeline.hpp"
#include "device.hpp"
#include "vulkan/vulkan_core.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>


Pipeline::Pipeline(Device& device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo) : device{device}{
  createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
}

std::vector<char> Pipeline::readFile(const std::string& filepath){


  // Ate flag means seek to end immediately, binary assures we read in as a binary 
  std::ifstream file{filepath, std::ios::ate | std::ios::binary};

  if (!file.is_open()){
    throw std::runtime_error("Failed to open file: " + filepath);
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();
  return buffer;

}



void Pipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo){
  auto vertCode = readFile(vertFilePath);
  auto fragCode = readFile(fragFilePath);

  std::cout << "Vertex Shader size: " << vertCode.size() << "\n";
  std::cout << "Fragment Shader size: " << fragCode.size() << "\n"; 
}



void Pipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

  if(vkCreateShaderModule(device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS){
    throw std::runtime_error("Failed to create shader module");
  }

}





PipelineConfigInfo Pipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height){
  PipelineConfigInfo configInfo;
  
  return configInfo;



}

