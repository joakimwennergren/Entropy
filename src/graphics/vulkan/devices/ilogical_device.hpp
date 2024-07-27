#pragma once

#include <vulkan/vulkan.hpp>
#include <servicelocators/servicelocator.hpp>

struct ILogicalDevice : public IService
{
    virtual ~ILogicalDevice() = default;
    virtual VkDevice Get() = 0;
    // QueueFamily index
    uint32_t queueFamiliy = 0;
};