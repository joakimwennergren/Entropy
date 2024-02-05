#include "setup.hpp"

std::shared_ptr<ServiceLocator> Setup()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    // Create the window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    auto _window = glfwCreateWindow(640, 340, "Entropy application", NULL, NULL);
    glfwHideWindow(_window);

    if (!_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Get initial window framebuffer size
    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);
    VkExtent2D frame = {
        .width = (uint32_t)width,
        .height = (uint32_t)height};

    // Create items for vulkan
    auto vkInstance = std::make_shared<VulkanInstance>("Tests");
    auto windowSurface = std::make_shared<WindowSurface>(vkInstance, _window);
    auto physicalDevice = std::make_shared<PhysicalDevice>(vkInstance, windowSurface);
    auto logicalDevice = std::make_shared<LogicalDevice>(physicalDevice, windowSurface);
    auto swapChain = std::make_shared<Swapchain>(physicalDevice->Get(), logicalDevice->Get(), windowSurface, frame);
    auto commandPool = std::make_shared<CommandPool>(logicalDevice, physicalDevice, windowSurface);
    auto descriptorPool = std::make_shared<DescriptorPool>(logicalDevice);

    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding textureLayoutBinding{};
    textureLayoutBinding.binding = 2;
    textureLayoutBinding.descriptorCount = 1;
    textureLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    textureLayoutBinding.pImmutableSamplers = nullptr;
    textureLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::vector<VkDescriptorSetLayoutBinding> bindings = {uboLayoutBinding, samplerLayoutBinding, textureLayoutBinding};

    auto descriptorSetLayout = std::make_shared<DescriptorsetLayout>(logicalDevice, bindings);
    auto descriptorSet = std::make_shared<Descriptorset>(logicalDevice, descriptorPool, descriptorSetLayout);

    // Add services to service locator
    auto serviceLocator = std::make_shared<ServiceLocator>();
    serviceLocator->registerService("PhysicalDevice", physicalDevice);
    serviceLocator->registerService("LogicalDevice", logicalDevice);
    serviceLocator->registerService("DescriptorSet", descriptorSet);
    serviceLocator->registerService("SwapChain", swapChain);
    serviceLocator->registerService("CommandPool", commandPool);
    serviceLocator->registerService("VkInstance", vkInstance);
    serviceLocator->registerService("Surface", windowSurface);

    return serviceLocator;
}