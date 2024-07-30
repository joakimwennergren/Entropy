#pragma once

#include <vulkan/vulkan.hpp>
#include <servicelocators/servicelocator.hpp>

struct IPipelineCache : public IService
{
    virtual ~IPipelineCache() = default;
    virtual VkPipelineCache Get() = 0;
};