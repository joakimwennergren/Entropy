#include "context.hpp"

using namespace Symbios::Core;

#ifdef BUILD_FOR_IOS
/**
 * @brief Construct a new Context:: Context object
 *
 * @param layer
 * @param frame
 */
Context::Context(CA::MetalLayer *layer, CGRect frame)
{
    this->_layer = layer;
    this->_frame = frame;
    this->CreateInstance();
    this->CreateSurfaceiOS(layer);
    this->PickPhysicalDevice();
    this->CreateLogicalDevice();
    this->CreateSwapChain(frame);
    this->CreateImageViews();
    this->CreateCommandPool();
    this->CreateDescriptorPool();
    this->CreateDescriptorSetLayout();
    this->CreateTextureSampler();
}

#endif

#ifdef BUILD_FOR_WINDOWS
/**
 * @brief Construct a new Context:: Context object
 *
 * @param window
 */
Context::Context(GLFWwindow *window)
{
    this->_window = window;

    this->CreateInstance();
    this->CreateSurfaceWindows(window);
    this->PickPhysicalDevice();
    this->CreateLogicalDevice();
    this->CreateSwapChain(window);
    this->CreateImageViews();
    this->CreateCommandPool();
    this->CreateDescriptorPool();
    this->CreateDescriptorSetLayout();
    this->CreateTextureSampler();
}
#endif

#ifdef BUILD_FOR_LINUX
/**
 * @brief Construct a new Context:: Context object
 *
 * @param window
 */
Context::Context(GLFWwindow *window)
{
    this->_window = window;

    this->CreateInstance();
    this->CreateSurfaceLinux(window);
    this->PickPhysicalDevice();
    this->CreateLogicalDevice();
    this->CreateSwapChain(window);
    this->CreateImageViews();
    this->CreateCommandPool();
    this->CreateDescriptorPool();
    this->CreateDescriptorSetLayout();
    this->CreateTextureSampler();
}
#endif

#ifdef BUILD_FOR_MACOS
/**
 * @brief Construct a new Context:: Context object
 *
 * @param window
 */
Context::Context(GLFWwindow *window)
{
    this->_window = window;

    this->CreateInstance();
    this->CreateSurfaceMacOS(window);
    this->PickPhysicalDevice();
    this->CreateLogicalDevice();
    this->CreateSwapChain(window);
    this->CreateImageViews();
    this->CreateCommandPool();
    this->CreateDescriptorPool();
    this->CreateDescriptorSetLayout();
    this->CreateTextureSampler();
}
#endif

#ifdef BUILD_FOR_IOS
/**
 * @brief
 *
 * @param layer
 */
void Context::CreateSurfaceiOS(CA::MetalLayer *layer)
{
    VkIOSSurfaceCreateInfoMVK createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.pView = layer;

    if (vkCreateIOSSurfaceMVK(this->_instance, &createInfo, nullptr, &this->_surface) != VK_SUCCESS)
    {
        PLOG_FATAL << "Could not create iOS surface!";
        exit(EXIT_FAILURE);
    }
}
#endif

#ifdef BUILD_FOR_MACOS
void Context::CreateSurfaceMacOS(GLFWwindow *window)
{
    if (glfwCreateWindowSurface(this->_instance, window, nullptr, &this->_surface) != VK_SUCCESS)
    {
        PLOG_FATAL << "Could not create MacOS surface!";
        exit(EXIT_FAILURE);
    }
}
#endif

#ifdef BUILD_FOR_WINDOWS
/**
 * @brief
 *
 * @param window
 */
void Context::CreateSurfaceWindows(GLFWwindow *window)
{
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = glfwGetWin32Window(window);
    createInfo.hinstance = GetModuleHandle(nullptr);

    if (vkCreateWin32SurfaceKHR(this->_instance, &createInfo, nullptr, &this->_surface) != VK_SUCCESS)
    {
        PLOG_FATAL << "Could not create Windows surface!";
        exit(EXIT_FAILURE);
    }
}
#endif

#ifdef BUILD_FOR_LINUX
/**
 * @brief
 *
 * @param window
 */
void Context::CreateSurfaceLinux(GLFWwindow *window)
{
    if (glfwCreateWindowSurface(this->_instance, window, nullptr, &this->_surface) != VK_SUCCESS)
    {
        PLOG_FATAL << "Could not create linux surface!";
        exit(EXIT_FAILURE);
    }
}
#endif