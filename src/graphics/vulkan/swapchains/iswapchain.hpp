#pragma once

#include <vulkan/vulkan.hpp>
#include <servicelocators/servicelocator.hpp>

struct ISwapchain : public IService
{
    virtual ~ISwapchain() = default;
    virtual VkSwapchainKHR Get() = 0;
    virtual void RecreateSwapChain(int width, int height) = 0;
    VkExtent2D swapChainExtent;
    VkFormat swapChainImageFormat;
};