#pragma once

#include "ipipelinecache.hpp"

#include <graphics/vulkan/devices/ilogical_device.hpp>
#include <graphics/vulkan/utilities/utilities.hpp>
#include <spdlog/spdlog.h>

namespace Entropy::Graphics::Vulkan::Caches {
  struct PipelineCache final : ServiceBase<IPipelineCache> {
    /**
     * @brief Constructs a new PipelineCache object.
     *
     * This constructor initializes the PipelineCache by retrieving the logical device
     * through the ServiceLocator. It then creates a Vulkan pipeline cache using
     * vkCreatePipelineCache.
     *
     * @return A new instance of the PipelineCache class.
     */
    PipelineCache() {
      const ServiceLocator *sl = ServiceLocator::GetInstance();
      _logicalDevice = sl->getService<ILogicalDevice>();

      VkPipelineCacheCreateInfo pipelineCacheCreateInfo{};
      pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

      VK_CHECK(vkCreatePipelineCache(_logicalDevice->Get(),
        &pipelineCacheCreateInfo, nullptr,
        &_pipelineCache));
    }

    /**
     * @brief Destructor for the PipelineCache class.
     *
     * This method is responsible for cleaning up the Vulkan pipeline cache. It uses
     * the Vulkan function vkDestroyPipelineCache to destroy the pipeline cache associated
     * with the logical device. This is called automatically when an object of the
     * PipelineCache class goes out of scope.
     *
     * @note This destructor overrides the base class destructor to ensure proper
     * cleanup of Vulkan resources.
     */
    ~PipelineCache() override {
      vkDestroyPipelineCache(_logicalDevice->Get(), _pipelineCache, nullptr);
    }

    VkPipelineCache Get() override { return _pipelineCache; }

  private:
    // Pipeline cache handle
    VkPipelineCache _pipelineCache = VK_NULL_HANDLE;
    // Logical device dependency
    std::shared_ptr<ILogicalDevice> _logicalDevice;
  };
} // namespace Entropy::Graphics::Vulkan::Caches
