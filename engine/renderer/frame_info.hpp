#pragma once

#include "camera.hpp"
#include "game_object.hpp"
#include "../containers/array.hpp"
#include "../memory/linear_allocator.hpp"

#include <vulkan/vulkan.h>

#define MAX_LIGHTS 10 

namespace Engine { namespace Renderer {

using FrameGameObjectQueue =
  Containers::Array<GameObject*, Memory::LinearAllocator>;

struct PointLight {
  glm::vec4 position{};
  glm::vec4 color{};
};

struct GlobalUbo {
  glm::mat4 projection{1.0f};
  glm::mat4 view{1.0f};
  glm::vec4 ambientLightColor{1.0f, 1.0f, 1.0f, 0.02f};
  PointLight pointLights[MAX_LIGHTS];
  int numLights;
};


struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  Camera& camera;
  VkDescriptorSet globalDescriptorSet;
  FrameGameObjectQueue& renderables;
  FrameGameObjectQueue& pointLights;
};

} } // namespace Engine::Renderer
