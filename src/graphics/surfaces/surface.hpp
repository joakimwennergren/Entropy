#pragma once

#include <memory>
#include <iostream>
#include <config.hpp>

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX)
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#endif

#ifdef BUILD_FOR_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#undef max
#endif

#ifdef BUILD_FOR_IOS
#define VK_USE_PLATFORM_IOS_MVK
#include <vulkan/vulkan.hpp>
#include <MetalKit/MetalKit.hpp>
#endif

#include <graphics/instances/vk_instance.hpp>
#include <services/service.hpp>

using namespace Entropy::Graphics::Instances;
using namespace Entropy::Services;

namespace Entropy
{
    namespace Graphics
    {
        namespace Surfaces
        {
            class WindowSurface : public Service
            {
            public:
#ifdef BUILD_FOR_MACOS
                WindowSurface(std::shared_ptr<VulkanInstance> instance, GLFWwindow *window);
#endif
#ifdef BUILD_FOR_WINDOWS
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

                inline bool isValid() override { return true; };

            private:
                std::shared_ptr<VulkanInstance> _instance;
                VkSurfaceKHR _surface = VK_NULL_HANDLE;
            };
        }
    }
}
