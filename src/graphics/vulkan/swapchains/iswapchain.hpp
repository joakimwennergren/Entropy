#pragma once

#include <servicelocators/servicelocator.hpp>
#include <vulkan/vulkan.hpp>

struct ISwapchain : public IService {
  virtual ~ISwapchain() = default;
  virtual VkSwapchainKHR Get() = 0;
  virtual void RecreateSwapChain(int width, int height) = 0;
  virtual void Build(VkSurfaceKHR surface, VkExtent2D frame,
                     VkSwapchainKHR oldswapChain) = 0;
  VkExtent2D swapChainExtent;
  VkFormat swapChainImageFormat;
  std::vector<VkImage> _swapChainImages;
  VkSwapchainKHR _swapChain;
  std::vector<VkImageView> _swapChainImageViews;
};