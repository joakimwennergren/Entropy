#pragma once

#include <config.hpp>
#include <vulkan/vulkan.hpp>
#include <servicelocators/servicelocator.hpp>

struct IPhysicalDevice : IService {
    ~IPhysicalDevice() override = default;

    virtual VkPhysicalDevice Get() = 0;

#ifdef BUILD_FOR_MACOS
    std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME,
        "VK_KHR_portability_subset"
    };
#endif

#ifdef BUILD_FOR_IOS
    std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                                                  "VK_KHR_portability_subset",
                                                  "VK_EXT_metal_objects"};
#endif

#ifdef BUILD_FOR_WINDOWS
    std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#endif

#ifdef BUILD_FOR_ANDROID
    std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#endif
};
