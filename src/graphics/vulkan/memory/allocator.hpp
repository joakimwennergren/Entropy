#pragma once

#include "graphics/vulkan/devices/logical_device.hpp"
#include "graphics/vulkan/devices/physical_device.hpp"
#include "graphics/vulkan/instances/vk_instance.hpp"
#include "spdlog/spdlog.h"
#include "vk_mem_alloc.h"
#include "vulkan/vulkan_core.h"

using namespace Entropy::Graphics::Vulkan::Instances;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Memory {

struct Allocator {

  Allocator(VulkanInstance i, PhysicalDevice pd, LogicalDevice ld)
      : _instance{i}, _physicalDevice{pd}, _logicalDevice{ld} {

    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
    allocatorCreateInfo.physicalDevice = _physicalDevice.Get();
    allocatorCreateInfo.device = _logicalDevice.Get();
    allocatorCreateInfo.instance = _instance.Get();
    allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

    if (vmaCreateAllocator(&allocatorCreateInfo, &_allocator) != VK_SUCCESS) {
      spdlog::error("Couldn't create allocator");
    }
  }

  //~Allocator() { vmaDestroyAllocator(_allocator); }

  inline VmaAllocator Get() { return _allocator; };

private:
  VulkanInstance _instance;
  Devices::PhysicalDevice _physicalDevice;
  Devices::LogicalDevice _logicalDevice;

  VmaAllocator _allocator = VK_NULL_HANDLE;
};
} // namespace Memory
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy