#pragma once

#include <graphics/vulkan/instances/ivk_instance.hpp>
#include <graphics/vulkan/devices/ilogical_device.hpp>
#include <graphics/vulkan/devices/iphysical_device.hpp>
#include <graphics/vulkan/utilities/helpers.hpp>

#include "iallocator.hpp"
#include "spdlog/spdlog.h"
#include "vk_mem_alloc.h"
#include <servicelocators/servicelocator.hpp>

namespace Entropy::Graphics::Vulkan::Memory {
  struct Allocator final : ServiceBase<IAllocator> {
    /**
     * Allocator constructor initializes the VMA (Vulkan Memory Allocator) with the required settings and Vulkan functions.
     *
     * The constructor retrieves instances of Vulkan-related services (VulkanInstance, PhysicalDevice, LogicalDevice)
     * from the ServiceLocator. It sets up the VMA Vulkan functions and creates a VMA allocator instance with the
     * necessary settings such as flags and Vulkan API version.
     *
     * @return No return value but initializes the internal VMA allocator (_allocator).
     */
    Allocator() {
      const ServiceLocator *sl = ServiceLocator::GetInstance();
      const auto vulkanInstance = sl->getService<IVulkanInstance>();
      const auto physicalDevice = sl->getService<IPhysicalDevice>();
      const auto logicalDevice = sl->getService<ILogicalDevice>();

      VmaVulkanFunctions vulkanFunctions = {};
      vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
      vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

      VmaAllocatorCreateInfo allocatorCreateInfo = {};
      allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
      allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
      allocatorCreateInfo.physicalDevice = physicalDevice->Get();
      allocatorCreateInfo.device = logicalDevice->Get();
      allocatorCreateInfo.instance = vulkanInstance->Get();
      allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

      VK_CHECK(vmaCreateAllocator(&allocatorCreateInfo, &_allocator));
    }

    /**
     * Destructor for the Allocator class that cleans up the VMA (Vulkan Memory Allocator) instance.
     *
     * This destructor calls the VMA function `vmaDestroyAllocator` to release all resources associated
     * with the internal VMA allocator (`_allocator`). This ensures that all memory allocated through
     * the allocator is properly freed and that no memory leaks occur.
     *
     * @return No return value.
     */
    ~Allocator() override { vmaDestroyAllocator(_allocator); }

    VmaAllocator Get() override { return _allocator; }

  private:
    VmaAllocator _allocator = VK_NULL_HANDLE;
  };
} // namespace Entropy::Graphics::Vulkan::Memory
