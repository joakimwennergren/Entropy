#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <config.hpp>

#include <graphics/instances/vk_instance.hpp>

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX) || defined(BUILD_FOR_WINDOWS)
#include <GLFW/glfw3.h>
#endif

#ifdef BUILD_FOR_IOS
#define VK_USE_PLATFORM_IOS_MVK
#include <vulkan/vulkan.hpp>
#include <MetalKit/MetalKit.hpp>
#endif

using namespace Entropy::Graphics::Instances;

namespace Entropy
{
    namespace Graphics
    {
        namespace Surfaces
        {
            class WindowSurface
            {
            public:
#ifdef BUILD_FOR_MACOS
                WindowSurface(std::shared_ptr<VulkanInstance> instance, GLFWwindow *window);
#endif
#ifdef BUILD_FOR_IOS
                WindowSurface(std::shared_ptr<VulkanInstance> instance, CA::MetalLayer *layer);
#endif
                ~WindowSurface();

                inline VkSurfaceKHR Get()
                {
                    return _surface;
                };

            private:
                std::shared_ptr<VulkanInstance> _instance;
                VkSurfaceKHR _surface = VK_NULL_HANDLE;
            };
        }
    }
}
