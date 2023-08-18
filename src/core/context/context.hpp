#pragma once

#define VK_USE_PLATFORM_IOS_MVK
#include <vulkan/vulkan.hpp>
#include <plog/Log.h>
#include "config.h"

#include <iostream>

#include <optional>
#include <set>

#include <MetalKit/MetalKit.hpp>

namespace Symbios
{
    namespace Core
    {
        /**
         * @brief
         *
         */
        class Context
        {
        public:
            Context(CA::MetalLayer *layer);
            ~Context();

            // Surface
            void CreateSurface(CA::MetalLayer *layer);

        private:
            void CreateInstance();

        private:
            // Validation Layers
            bool CheckValidationLayerSupport();
            VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
            void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);

            // Physical Device
            void PickPhysicalDevice();
            bool IsDeviceSuitable(VkPhysicalDevice device);

            // Logical device
            void CreateLogicalDevice();

            // Queues
            struct QueueFamilyIndices
            {
                std::optional<uint32_t> graphicsFamily;
                std::optional<uint32_t> presentFamily;

                bool isComplete()
                {
                    return graphicsFamily.has_value() && presentFamily.has_value();
                }
            };
            QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

        private:
            // Instance and Debugmessenger
            VkInstance _instance;
            VkDebugUtilsMessengerEXT _debugMessenger;

            // Devices
            VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
            VkDevice _device;

            // Queues
            VkQueue _graphicsQueue;
            VkQueue _presentQueue;

            // Surface
            VkSurfaceKHR _surface;
        };
    }

}
