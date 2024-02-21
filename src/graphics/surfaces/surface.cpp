#include "surface.hpp"

using namespace Entropy::Graphics::Surfaces;

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX)
WindowSurface::WindowSurface(std::shared_ptr<VulkanInstance> instance, GLFWwindow *window)
{

    this->window = window;

    if (instance == nullptr)
    {
        return;
    }

    _instance = instance;

    if (window == nullptr)
    {
        return;
    }

    if (glfwCreateWindowSurface(instance->Get(), window, nullptr, &_surface) != VK_SUCCESS)
    {
        std::cout << "Failed to create a window surface for platform 'MacOS'" << std::endl;
        exit(EXIT_FAILURE);
    }
}
#endif

#ifdef BUILD_FOR_WINDOWS
WindowSurface::WindowSurface(std::shared_ptr<VulkanInstance> instance, GLFWwindow *window)
{
    this->window = window;
    if (instance == nullptr)
    {
        return;
    }

    _instance = instance;

    if (window == nullptr)
    {
        return;
    }

    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = glfwGetWin32Window(window);
    createInfo.hinstance = GetModuleHandle(nullptr);

    if (vkCreateWin32SurfaceKHR(instance->Get(), &createInfo, nullptr, &this->_surface) != VK_SUCCESS)
    {
        exit(EXIT_FAILURE);
    }
}
#endif

#ifdef BUILD_FOR_IOS
WindowSurface::WindowSurface(std::shared_ptr<VulkanInstance> instance, CA::MetalLayer *layer)
{
    if (instance == nullptr)
    {
        return;
    }

    _instance = instance;

    if (layer == nullptr)
    {
        return;
    }

    VkIOSSurfaceCreateInfoMVK createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.pView = layer;

    if (vkCreateIOSSurfaceMVK(instance->Get(), &createInfo, nullptr, &_surface) != VK_SUCCESS)
    {
        std::cout << "Failed to create a window surface for platform 'iOS'" << std::endl;
        exit(EXIT_FAILURE);
    }
}
#endif

WindowSurface::~WindowSurface()
{
    if (_instance != nullptr)
    {
        vkDestroySurfaceKHR(_instance->Get(), _surface, nullptr);
    }
}