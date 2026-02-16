#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "device.hpp"
#include "vulkan/vulkan_core.h"


struct PipelineConfigInfo {
  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

class Pipeline{

public:
  Pipeline(Device& device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);

  ~Pipeline();

  Pipeline(const Pipeline &) = delete;

  Pipeline &operator=(const Pipeline&) = delete;


  static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);


private:
  static std::vector<char> readFile(const std::string& filepath);


  void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);


  void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);



  Device& device;
  VkPipeline graphicsPipeline;
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;



};
