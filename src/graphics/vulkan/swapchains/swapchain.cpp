#include "swapchain.hpp"

using namespace Entropy::Graphics::Vulkan::SwapChains;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::QueueFamilies;

SwapChain::SwapChain() {
    const ServiceLocator *sl = ServiceLocator::GetInstance();
    _physicalDevice = sl->getService<IPhysicalDevice>();
    _logicalDevice = sl->getService<ILogicalDevice>();
}

void SwapChain::Build(const VkSurfaceKHR surface, const VkExtent2D frame,
                      const VkSwapchainKHR oldSwapChain) {
    _surface = surface;

    auto [capabilities, formats, presentModes] =
            QuerySwapChainSupport(_physicalDevice->Get(), surface);

    auto [format, colorSpace] =
            ChooseSwapSurfaceFormat(formats);

    auto [graphicsFamily, presentFamily] =
            QueueFamily::FindQueueFamilies(_physicalDevice->Get(), surface);

    const uint32_t queueFamilyIndices[] = {
        graphicsFamily.value(),
        presentFamily.value()
    };

    const VkPresentModeKHR presentMode =
            ChooseSwapPresentMode(presentModes);

    const VkExtent2D extent = ChooseSwapExtent(capabilities, frame);

    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 &&
        imageCount > capabilities.maxImageCount) {
        imageCount = capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = format;
    createInfo.imageColorSpace = colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (graphicsFamily != presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }
    createInfo.preTransform = capabilities.currentTransform;

#ifdef BUILD_FOR_WINDOWS
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
#else
    createInfo.compositeAlpha =
            VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
#endif
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = oldSwapChain;

    VK_CHECK(vkCreateSwapchainKHR(_logicalDevice->Get(), &createInfo, nullptr,
        &_swapChain));

    swapChainImageFormat = format;
    swapChainExtent = extent;

    imageCount = capabilities.minImageCount + 1;

    vkGetSwapchainImagesKHR(_logicalDevice->Get(), _swapChain, &imageCount,
                            nullptr);

    _swapChainImages.resize(imageCount);

    vkGetSwapchainImagesKHR(_logicalDevice->Get(), _swapChain, &imageCount,
                            _swapChainImages.data());

    _swapChainImageViews.resize(_swapChainImages.size());

    for (uint32_t i = 0; i < _swapChainImages.size(); i++) {
        _swapChainImageViews[i] = std::make_shared<ImageView>(_swapChainImages[i], swapChainImageFormat);
    }
}

void SwapChain::RecreateSwapChain(const uint32_t width, const uint32_t height) {
    swapChainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;
    swapChainExtent =
            VkExtent2D{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
    _swapChainImageViews.clear();
    _swapChainImages.clear();
    Build(_surface, swapChainExtent, _swapChain);
}

VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats) {
    const auto colorFormat = GetColorFormat();
    for (const auto &availableFormat: availableFormats) {
        if (availableFormat.format == colorFormat &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR SwapChain::ChooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes) {
    for (const auto &availablePresentMode: availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities,
                                       VkExtent2D frame) {
    if (capabilities.currentExtent.width !=
        std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }

    VkExtent2D actualExtent = {
        (frame.width),
        (frame.height)
    };

    actualExtent.width =
            std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                       capabilities.maxImageExtent.width);
    actualExtent.height =
            std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                       capabilities.maxImageExtent.height);

    return actualExtent;
}

SwapChainSupportDetails SwapChain::QuerySwapChainSupport(VkPhysicalDevice device,
                                                         VkSurfaceKHR surface) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                              &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                         nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                             details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface,
                                              &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}
