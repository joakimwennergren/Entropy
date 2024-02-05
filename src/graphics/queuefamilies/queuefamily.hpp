#pragma once

#include <optional>
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <config.hpp>
#include <graphics/instances/vk_instance.hpp>

using namespace Entropy::Graphics::Instances;

namespace Entropy
{
    namespace Graphics
    {
        namespace QueueFamilies
        {
            struct QueueFamilyIndices
            {
                std::optional<uint32_t> graphicsFamily;
                std::optional<uint32_t> presentFamily;

                bool isComplete()
                {
                    return graphicsFamily.has_value() && presentFamily.has_value();
                }
            };

            class QueueFamily
            {
            public:
                static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
                QueueFamily();

            private:
            };
        }
    }
}
