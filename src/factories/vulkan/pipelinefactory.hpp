#pragma once
#include "../../factories/vulkan/descriptorset_factory.hpp"
#include "../../factories/vulkan/descriptorsetlayout_factory.hpp"
#include "../../graphics/vulkan/descriptorpools/descriptorpool.hpp"
#include "../../graphics/vulkan/vulkan_backend.hpp"
#include <graphics/vulkan/pipelines/static_pipeline.hpp>

using namespace Entropy::Graphics::Vulkan;
using namespace Entropy::Graphics::Vulkan::Buffers;

namespace Entropy {
namespace Factories {
namespace Vulkan {

struct PipelineFactory {

  PipelineFactory(VulkanBackend backend, RenderPass renderPass,
                  Swapchain swapchain, DescriptorPool descriptorPool,
                  DescriptorSetLayoutFactory dslf, DescriptorSetFactory dsf)
      : _vkBackend{backend}, _renderPass{renderPass}, _swapChain{swapchain},
        _descriptorPool{descriptorPool}, _descriptorSetLayoutFactory{dslf},
        _descriptorSetFactory{dsf} {}

  StaticPipeline *CreateStaticPipeline() {
    return new StaticPipeline(_vkBackend, _renderPass, _swapChain,
                              _descriptorPool, _descriptorSetLayoutFactory,
                              _descriptorSetFactory);
  }

private:
  VulkanBackend _vkBackend;
  RenderPass _renderPass;
  Swapchain _swapChain;
  DescriptorPool _descriptorPool;
  DescriptorSetLayoutFactory _descriptorSetLayoutFactory;
  DescriptorSetFactory _descriptorSetFactory;
};

} // namespace Vulkan
} // namespace Factories
} // namespace Entropy