#pragma once

#include <vulkan/vulkan.hpp>
#include <plog/Log.h>
#include "config.h"

/**
 * @brief
 *
 */
class Context
{
public:
    Context();
    ~Context();

private:
    void CreateInstance();

private:
    bool CheckValidationLayerSupport();
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);

private:
    VkInstance _instance;
    VkDebugUtilsMessengerEXT _debugMessenger;
};