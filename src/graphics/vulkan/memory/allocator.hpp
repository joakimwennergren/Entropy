#pragma once

#include <graphics/vulkan/instances/ivk_instance.hpp>
#include <graphics/vulkan/devices/ilogical_device.hpp>
#include <graphics/vulkan/devices/iphysical_device.hpp>

#include "iallocator.hpp"
#include "spdlog/spdlog.h"
#include "vk_mem_alloc.h"
#include <servicelocators/servicelocator.hpp>

namespace Entropy
{
  namespace Graphics
  {
    namespace Vulkan
    {
      namespace Memory
      {
        struct Allocator : public ServiceBase<IAllocator>
        {
          Allocator()
          {
            ServiceLocator *sl = ServiceLocator::GetInstance();
            auto vulkanInstance = sl->getService<IVulkanInstance>();
            auto physicalDevice = sl->getService<IPhysicalDevice>();
            auto logicalDevice = sl->getService<ILogicalDevice>();

            assert(vulkanInstance == nullptr);
            assert(physicalDevice == nullptr);
            assert(logicalDevice == nullptr);

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

            if (vmaCreateAllocator(&allocatorCreateInfo, &_allocator) != VK_SUCCESS)
            {
              spdlog::error("Couldn't create allocator");
            }
          }

          ~Allocator() { vmaDestroyAllocator(_allocator); }

          inline VmaAllocator Get() { return _allocator; }

        private:
          VmaAllocator _allocator = VK_NULL_HANDLE;
        };
      } // namespace Memory
    } // namespace Vulkan
  } // namespace Graphics
} // namespace Entropy