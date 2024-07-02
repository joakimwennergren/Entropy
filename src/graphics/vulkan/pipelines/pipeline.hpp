#pragma once

#include <array>
#include <utility>

#include <factories/vulkan/descriptorset_factory.hpp>
#include <factories/vulkan/descriptorsetlayout_factory.hpp>
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

#include "spdlog/spdlog.h"

using namespace Entropy::Filesystem;
using namespace Symbios::Graphics::Vulkan::Shaders;
using namespace Entropy::Graphics::Vulkan::RenderPasses;
using namespace Entropy::Graphics::Vulkan::Swapchains;
using namespace Entropy::Graphics::Vulkan::Descriptorsets;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Factories::Vulkan;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Pipelines {
class Pipeline {
public:
  Pipeline(VulkanBackend vbe, RenderPass rp, Swapchain sc, DescriptorPool dp,
           DescriptorSetLayoutFactory dslf, DescriptorSetFactory dsf);
  ~Pipeline();

  void Build(Shader shader, std::vector<VkDescriptorSetLayout> dsLayouts);

  // void Build(const std::string name, const std::string vertexShader,
  //            const std::string fragmentShader,
  //            std::vector<VkDescriptorSetLayout> dsLayout, bool depthWrite,
  //            glm::vec2 depthBounds, VkPipelineLayoutCreateInfo
  //            pipelinelayout, VkPolygonMode polygonMode);

  // void Build(const std::string name, std::vector<char> vert_shader,
  //            std::vector<char> frag_shader,
  //            std::vector<VkDescriptorSetLayout> dsLayout, bool depthWrite,
  //            glm::vec2 depthBounds, VkPipelineLayoutCreateInfo
  //            pipelinelayout, VkPolygonMode polygonMode);

  inline VkPipeline GetPipeline() { return _pipeline; };
  inline VkPipelineLayout GetPipelineLayout() { return _pipelineLayout; };

  std::vector<Descriptorset> descriptorSets;

protected:
  // PipelineLayout and pipeline
  VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
  VkPipeline _pipeline = VK_NULL_HANDLE;

  // Depedencies
  VulkanBackend *_vulkanBackend = nullptr;
  RenderPass *_renderPass = nullptr;
  Swapchain *_swapChain = nullptr;
  DescriptorPool *_descriptorPool = nullptr;
  DescriptorSetLayoutFactory *_descriptorSetLayoutFactory = nullptr;
  DescriptorSetFactory *_descriptorSetFactory = nullptr;
};
} // namespace Pipelines
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
