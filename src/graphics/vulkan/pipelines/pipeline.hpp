#pragma once

#include <array>
#include <utility>

#include <data/vertex.hpp>
#include <filesystem/filesystem.hpp>
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>
#include <graphics/vulkan/descriptorsetlayouts/descriptorsetlayout.hpp>
#include <graphics/vulkan/descriptorsets/descriptorset.hpp>
#include <graphics/vulkan/devices/logical_device.hpp>
#include <graphics/vulkan/pipelinecaches/pipelinecache.hpp>
#include <graphics/vulkan/renderpasses/renderpass.hpp>
#include <graphics/vulkan/shaders/shader.hpp>
#include <graphics/vulkan/swapchains/swapchain.hpp>
#include <spdlog/spdlog.h>

using namespace Entropy::Filesystem;
using namespace Entropy::Graphics::Vulkan::Shaders;
using namespace Entropy::Graphics::Vulkan::RenderPasses;
using namespace Entropy::Graphics::Vulkan::Swapchains;
using namespace Entropy::Graphics::Vulkan::Descriptorsets;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Data;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Pipelines {
class Pipeline {
public:
  Pipeline(std::shared_ptr<RenderPass> renderPass) {
    ServiceLocator *sl = ServiceLocator::GetInstance();
    _logicalDevice = sl->getService<ILogicalDevice>();
    _swapchain = sl->getService<ISwapchain>();
    _pipelineCache = sl->getService<IPipelineCache>();
    _renderPass = renderPass;
  }

  ~Pipeline() {
    vkDestroyPipeline(_logicalDevice->Get(), _pipeline, nullptr);
    vkDestroyPipelineLayout(_logicalDevice->Get(), _pipelineLayout, nullptr);
  }

  void Build(std::shared_ptr<Shader> shader,
             std::vector<VkDescriptorSetLayout> dsLayouts);

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
  std::shared_ptr<Shader> _shader;
  std::shared_ptr<ILogicalDevice> _logicalDevice;
  std::shared_ptr<ISwapchain> _swapchain;
  std::shared_ptr<RenderPass> _renderPass;
  std::shared_ptr<IPipelineCache> _pipelineCache;
};
} // namespace Pipelines
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
