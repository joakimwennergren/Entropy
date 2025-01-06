#pragma once

#include <vulkan/vulkan.hpp>
#include <servicelocators/servicelocator.hpp>

class IVulkanInstance : public IService
{
public:
    virtual ~IVulkanInstance() = default;
    virtual VkInstance Get() = 0;
};