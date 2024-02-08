#include "vk_instance.hpp"

using namespace Entropy::Graphics::Instances;

VulkanInstance::VulkanInstance(std::string applicationName)
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = applicationName.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pEngineName = "Entropy";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

#ifdef BUILD_FOR_MACOS
    createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensionProps(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProps.data());

    std::vector<const char *> extensions;
    for (const auto &extension : extensionProps)
    {
        extensions.push_back(extension.extensionName);
    }

#ifdef BUILD_FOR_MACOS
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

#ifdef BUILD_FOR_ANDROID
    extensions.push_back("VK_KHR_surface");
    extensions.push_back("VK_KHR_android_surface");
    // extensions.push_back("VK_KHR_surface_protected_capabilities");
#endif

    createInfo.enabledExtensionCount = (uint32_t)extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (ValidationLayer::CheckValidationLayerSupport(_validationLayers))
    {
        createInfo.enabledLayerCount = (uint32_t)_validationLayers.size();
        createInfo.ppEnabledLayerNames = _validationLayers.data();
    }

    if (vkCreateInstance(&createInfo, nullptr, &this->_instance) != VK_SUCCESS)
    {
        std::cout << "Failed to create vulkan instance!" << std::endl;
        exit(EXIT_FAILURE);
    }
}