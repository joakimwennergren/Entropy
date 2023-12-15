#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <config.hpp>

#include <graphics/surfaces/surface.hpp>
#include <graphics/queuefamilies/queuefamily.hpp>
#include <services/service.hpp>

using namespace Entropy::Graphics::Surfaces;
using namespace Entropy::Graphics::QueueFamilies;
using namespace Entropy::Services;

namespace Entropy
{
    namespace Graphics
    {
        namespace Swapchains
        {
            struct SwapChainSupportDetails
            {
                VkSurfaceCapabilitiesKHR capabilities;
                std::vector<VkSurfaceFormatKHR> formats;
                std::vector<VkPresentModeKHR> presentModes;
            };

            class Swapchain : public Service
            {
            public:
                Swapchain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, std::shared_ptr<WindowSurface> surface, VkExtent2D frame);
                ~Swapchain();
                static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
                std::vector<VkImageView> swapChainImageViews;
                std::vector<VkImage> swapChainImages;
                VkExtent2D swapChainExtent;
                VkFormat swapChainImageFormat;
                inline VkSwapchainKHR Get() { return _swapChain; };
                bool isValid() override { return _swapChain != nullptr; };

            private:
                VkSwapchainKHR _swapChain;
                VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
                VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
                VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, VkExtent2D frame);
                VkDevice _logicalDevice;
            };
        }
    }
}
