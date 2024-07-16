#pragma once

#include <config.hpp>
#include <iostream>
#include <memory>

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX)
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
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
#endif

#ifdef BUILD_FOR_ANDROID
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_android.h>
#endif

#include <graphics/vulkan/instances/vk_instance.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>

using namespace Entropy::Graphics::Vulkan::Instances;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Surfaces {

class WindowSurface {
public:
#ifdef BUILD_FOR_MACOS
  WindowSurface(std::shared_ptr<VulkanInstance> instance, GLFWwindow *window);
  GLFWwindow *window;
#endif
#ifdef BUILD_FOR_WINDOWS
  WindowSurface(std::shared_ptr<VulkanInstance> instance, GLFWwindow *window);
  GLFWwindow *window;
#endif
#ifdef BUILD_FOR_IOS

#endif
  ~WindowSurface();

  inline VkSurfaceKHR Get() { return _surface; };
  VkSurfaceKHR _surface = VK_NULL_HANDLE;

private:
  std::shared_ptr<VulkanInstance> _instance;
};
} // namespace Surfaces
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy