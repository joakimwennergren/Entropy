#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"
#include "allocator.hpp"

using namespace Entropy::Graphics::Memory;

Allocator::Allocator(std::shared_ptr<VulkanInstance> instance, std::shared_ptr<LogicalDevice> logicalDevice, std::shared_ptr<PhysicalDevice> phyiscalDevice)
{
    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_1;
    allocatorCreateInfo.physicalDevice = phyiscalDevice->Get();
    allocatorCreateInfo.device = logicalDevice->Get();
    allocatorCreateInfo.instance = instance->Get();
    allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

    vmaCreateAllocator(&allocatorCreateInfo, &_allocator);
}

Allocator::~Allocator()
{
    vmaDestroyAllocator(_allocator);
}