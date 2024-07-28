#pragma once

#include "ipipelinecache.hpp"
#include "spdlog/spdlog.h"

namespace Entropy
{
  namespace Graphics
  {
    namespace Vulkan
    {
      namespace Caches
      {

        struct PipelineCache : public ServiceBase<IPipelineCache>
        {

          PipelineCache()
          {
            ServiceLocator *sl = ServiceLocator::GetInstance();
            auto logicalDevice = sl->getService<ILogicalDevice>();

            VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
            pipelineCacheCreateInfo.sType =
                VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
            vkCreatePipelineCache(logicalDevice->Get(), &pipelineCacheCreateInfo,
                                  nullptr, &_pipelineCache);
          }

          inline VkPipelineCache Get() { return _pipelineCache; };

        private:
          VkPipelineCache _pipelineCache = VK_NULL_HANDLE;
        };
      } // namespace Caches
    } // namespace Vulkan
  } // namespace Graphics
} // namespace Entropy