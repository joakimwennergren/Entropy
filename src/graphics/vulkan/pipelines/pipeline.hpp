#pragma once

#include <array>
#include <data/vertex.hpp>
#include <data/pushcontants.hpp>
#include <filesystem/filesystem.hpp>
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>
#include <graphics/vulkan/descriptorsets/descriptorset.hpp>
#include <graphics/vulkan/pipelinecaches/pipelinecache.hpp>
#include <graphics/vulkan/renderpasses/renderpass.hpp>
#include <graphics/vulkan/shaders/shader.hpp>
#include <graphics/vulkan/swapchains/swapchain.hpp>

using namespace Entropy::Filesystem;
using namespace Entropy::Graphics::Vulkan::Shaders;
using namespace Entropy::Graphics::Vulkan::RenderPasses;
using namespace Entropy::Graphics::Vulkan::Swapchains;
using namespace Entropy::Graphics::Vulkan::DescriptorSets;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Data;

namespace Entropy::Graphics::Vulkan::Pipelines {
  class Pipeline {
    /**
     * Constructs a Pipeline object.
     *
     * @param renderPass A shared pointer to a RenderPass object that this pipeline will use.
     * @return A constructed Pipeline object.
     */
  public:
    explicit Pipeline(const std::shared_ptr<RenderPass> &renderPass) {
      const ServiceLocator *sl = ServiceLocator::GetInstance();
      _logicalDevice = sl->getService<ILogicalDevice>();
      _swapChain = sl->getService<ISwapchain>();
      _pipelineCache = sl->getService<IPipelineCache>();
      _renderPass = renderPass;
    }

    /**
     * Destructor for the Pipeline class.
     *
     * This method cleans up Vulkan pipeline resources by destroying the pipeline
     * and pipeline layout using the logical device.
     */
    ~Pipeline() {
      vkDestroyPipeline(_logicalDevice->Get(), _pipeline, nullptr);
      vkDestroyPipelineLayout(_logicalDevice->Get(), _pipelineLayout, nullptr);
    }

    /**
     * Builds a new pipeline with the given shader and descriptor set layouts.
     *
     * @param shader A shared pointer to a Shader object that will be used by the pipeline.
     * @param dsLayouts A vector containing the descriptor set layouts to be used by the pipeline.
     */
    void Build(const std::shared_ptr<Shader> &shader,
               std::vector<VkDescriptorSetLayout> dsLayouts);

    [[nodiscard]] VkPipeline GetPipeline() const { return _pipeline; };
    [[nodiscard]] VkPipelineLayout GetPipelineLayout() const { return _pipelineLayout; };

    std::vector<DescriptorSet> descriptorSets;

  protected:
    VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
    VkPipeline _pipeline = VK_NULL_HANDLE;
    std::shared_ptr<Shader> _shader;
    std::shared_ptr<ILogicalDevice> _logicalDevice;
    std::shared_ptr<ISwapchain> _swapChain;
    std::shared_ptr<RenderPass> _renderPass;
    std::shared_ptr<IPipelineCache> _pipelineCache;
  };
} // namespace Entropy::Graphics::Vulkan::Pipelines
