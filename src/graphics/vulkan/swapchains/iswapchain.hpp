#pragma once

#include <vulkan/vulkan.hpp>
#include <servicelocators/servicelocator.hpp>

class ISwapchain : public IService
{
public:
    virtual ~ISwapchain() = default;
    virtual VkSwapchainKHR Get() = 0;
};