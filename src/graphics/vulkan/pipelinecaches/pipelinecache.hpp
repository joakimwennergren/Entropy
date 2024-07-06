#pragma once

#include "graphics/vulkan/devices/logical_device.hpp"
#include "spdlog/spdlog.h"

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Caches {

struct PipelineCache {

  PipelineCache(LogicalDevice ld) : _logicalDevice{ld} {

    VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
    pipelineCacheCreateInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    vkCreatePipelineCache(_logicalDevice.Get(), &pipelineCacheCreateInfo,
                          nullptr, &_pipelineCache);
  }

  inline VkPipelineCache Get() { return _pipelineCache; };

private:
  Devices::LogicalDevice _logicalDevice;

  VkPipelineCache _pipelineCache = VK_NULL_HANDLE;
};
} // namespace Caches
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy