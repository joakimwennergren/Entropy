#pragma once
#include "../../factories/vulkan/descriptorset_factory.hpp"
#include "../../factories/vulkan/descriptorsetlayout_factory.hpp"
#include "../../graphics/vulkan/descriptorpools/descriptorpool.hpp"
#include "../../graphics/vulkan/vulkan_backend.hpp"
#include <graphics/vulkan/pipelinecaches/pipelinecache.hpp>
#include <graphics/vulkan/pipelines/static_pipeline.hpp>

using namespace Entropy::Graphics::Vulkan;
using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::Caches;
using namespace Entropy::Graphics::Vulkan::Pipelines;

namespace Entropy {
namespace Factories {
namespace Vulkan {

struct PipelineFactory {

  PipelineFactory(VulkanBackend backend, RenderPass renderPass,
                  Swapchain swapchain, DescriptorPool descriptorPool,
                  DescriptorSetLayoutFactory dslf, DescriptorSetFactory dsf,
                  PipelineCache pc)
      : _vkBackend{backend}, _renderPass{renderPass}, _swapChain{swapchain},
        _descriptorPool{descriptorPool}, _descriptorSetLayoutFactory{dslf},
        _descriptorSetFactory{dsf}, _pipelineCache{pc} {}

  StaticPipeline *CreateStaticPipeline() {
    return new StaticPipeline(_vkBackend, _renderPass, _swapChain,
                              _descriptorPool, _descriptorSetLayoutFactory,
                              _descriptorSetFactory, _pipelineCache);
  }

private:
  VulkanBackend _vkBackend;
  RenderPass _renderPass;
  Swapchain _swapChain;
  DescriptorPool _descriptorPool;
  DescriptorSetLayoutFactory _descriptorSetLayoutFactory;
  DescriptorSetFactory _descriptorSetFactory;
  PipelineCache _pipelineCache;
};

} // namespace Vulkan
} // namespace Factories
} // namespace Entropy