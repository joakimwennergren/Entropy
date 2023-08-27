#include "utilities.hpp"

using namespace Symbios::Graphics::Utilities;

uint32_t Utility::FindMemoryTypeIndex(std::shared_ptr<Context> _context, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
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