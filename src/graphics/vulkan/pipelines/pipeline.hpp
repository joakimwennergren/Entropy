#pragma once

#include <array>
#include <utility>

#include <filesystem/filesystem.hpp>
#include <graphics/data/pushcontants.hpp>
#include <graphics/data/vertex.hpp>
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>
#include <graphics/vulkan/descriptorsetlayouts/descriptorsetlayout.hpp>
#include <graphics/vulkan/descriptorsets/descriptorset.hpp>
#include <graphics/vulkan/devices/logical_device.hpp>
#include <graphics/vulkan/renderpasses/renderpass.hpp>
#include <graphics/vulkan/shaders/shader.hpp>
#include <graphics/vulkan/swapchains/swapchain.hpp>
#include <servicelocators/servicelocator.hpp>

#include "spdlog/spdlog.h"

using namespace Entropy::Filesystem;
using namespace Symbios::Graphics::Vulkan::Shaders;
using namespace Entropy::Graphics::Vulkan::RenderPasses;
using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Vulkan::Swapchains;
using namespace Entropy::Graphics::Vulkan::Descriptorsets;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Graphics::Vulkan::Devices;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Pipelines {
class Pipeline {
public:
  Pipeline(std::shared_ptr<RenderPass> renderPass,
           std::shared_ptr<ServiceLocator> serviceLocator,
           VkPolygonMode polygonMode);
  ~Pipeline();

  void Setup(std::unique_ptr<Shader> shader,
             std::vector<VkDescriptorSetLayout> dsLayout, bool depthWrite,
             glm::vec2 depthBounds, VkPipelineLayoutCreateInfo pipelinelayout,
             VkPolygonMode polygonMode);

  void Build(const std::string name, const std::string vertexShader,
             const std::string fragmentShader,
             std::vector<VkDescriptorSetLayout> dsLayout, bool depthWrite,
             glm::vec2 depthBounds, VkPipelineLayoutCreateInfo pipelinelayout,
             VkPolygonMode polygonMode);
  void Build(const std::string name, std::vector<char> vert_shader,
             std::vector<char> frag_shader,
             std::vector<VkDescriptorSetLayout> dsLayout, bool depthWrite,
             glm::vec2 depthBounds, VkPipelineLayoutCreateInfo pipelinelayout,
             VkPolygonMode polygonMode);
  inline VkPipeline GetPipeline() { return _pipeline; };
  inline VkPipelineLayout GetPipelineLayout() { return _pipelineLayout; };
  std::vector<std::shared_ptr<Descriptorset>> descriptorSets;

protected:
  VkPipelineLayout _pipelineLayout;
  VkPipeline _pipeline;
  std::shared_ptr<RenderPass> _renderPass;
  std::shared_ptr<LogicalDevice> _logicalDevice;
  std::shared_ptr<Swapchain> _swapchain;
  std::shared_ptr<DescriptorsetLayout> _descriptorSetLayout;
  std::shared_ptr<DescriptorPool> _descriptorPool;
  std::shared_ptr<ServiceLocator> _serviceLocator;
};
} // namespace Pipelines
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
