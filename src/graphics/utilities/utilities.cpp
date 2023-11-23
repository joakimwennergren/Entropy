#include "utilities.hpp"

using namespace Symbios::Graphics::Utilities;

uint32_t Utility::FindMemoryTypeIndex(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{

    // Store vulkan ctx
    auto _context = VulkanContext::GetInstance()->GetVulkanContext();

    // Get the physical device's memory properties
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(_context->GetPhysicalDevice(), &memProperties);

    // Iterate over memoryproperties and return index of matched property
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    // If none find print error and exit
    PLOG_ERROR << "Failed to find memory type index!";
    exit(EXIT_FAILURE);
}

void *Utility::AlignedAlloc(size_t size, size_t alignment)
{
    void *data = nullptr;
#if defined(_MSC_VER) || defined(__MINGW32__)
    data = _aligned_malloc(size, alignment);
#else
    int res = posix_memalign(&data, alignment, size);
    if (res != 0)
        data = nullptr;
#endif
    return data;
}

void Utility::AlignedFree(void *data)
{
#if defined(_MSC_VER) || defined(__MINGW32__)
    _aligned_free(data);
#else
    free(data);
#endif
}
