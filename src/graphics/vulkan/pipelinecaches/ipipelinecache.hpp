#ifndef __IPIPELINECACHE_HPP
#define __IPIPELINECACHE_HPP

#include <vulkan/vulkan.hpp>
#include <servicelocators/servicelocator.hpp>

struct IPipelineCache : public IService
{
    virtual ~IPipelineCache() = default;
    virtual VkPipelineCache Get() = 0;
};

#endif /* __IPIPELINECACHE_HPP */
