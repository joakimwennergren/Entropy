#ifndef __PIPELINECACHE_HPP
#define __PIPELINECACHE_HPP

#include "ipipelinecache.hpp"

#include <graphics/vulkan/devices/ilogical_device.hpp>
#include <graphics/vulkan/utilities/utilities.hpp>
#include <spdlog/spdlog.h>

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Caches {
struct PipelineCache : public ServiceBase<IPipelineCache> {
  // Constructor
  PipelineCache() {
    ServiceLocator *sl = ServiceLocator::GetInstance();
    _logicalDevice = sl->getService<ILogicalDevice>();

    VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO};

    VK_CHECK(vkCreatePipelineCache(_logicalDevice->Get(),
                                   &pipelineCacheCreateInfo, nullptr,
                                   &_pipelineCache));
  }
  // Destructor
  ~PipelineCache() {
    vkDestroyPipelineCache(_logicalDevice->Get(), _pipelineCache, nullptr);
  }

  // Gets the pipelinecache handle
  inline VkPipelineCache Get() { return _pipelineCache; };

private:
  // Pipelinecache handle
  VkPipelineCache _pipelineCache = VK_NULL_HANDLE;
  // Logical device dependency
  std::shared_ptr<ILogicalDevice> _logicalDevice;
};
} // namespace Caches
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy

#endif /* __PIPELINECACHE_HPP */
