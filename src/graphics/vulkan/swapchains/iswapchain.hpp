#pragma once

#include <servicelocators/servicelocator.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <vulkan/vulkan.hpp>

using namespace Entropy::Graphics::Vulkan::ImageViews;

struct ISwapchain : public IService {
    ~ISwapchain() override = default;

    virtual VkSwapchainKHR Get() = 0;

    virtual void RecreateSwapChain(int width, int height) = 0;

    virtual void Build(VkSurfaceKHR surface, VkExtent2D frame,
                       VkSwapchainKHR oldswapChain) = 0;

    VkExtent2D swapChainExtent;
    VkFormat swapChainImageFormat;
    std::vector<VkImage> _swapChainImages;
    VkSwapchainKHR _swapChain;
    std::vector<std::shared_ptr<ImageView> > _swapChainImageViews;
};
