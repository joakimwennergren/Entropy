#pragma once

#include "graphics/vulkan/devices/logical_device.hpp"
#include "graphics/vulkan/devices/physical_device.hpp"
#include "graphics/vulkan/instances/vk_instance.hpp"
#include <graphics/vulkan/memory/allocator.hpp>

#include <servicelocators/servicelocator.hpp>

#include <spdlog/spdlog.h>

namespace Entropy
{
  namespace Graphics
  {
    namespace Vulkan
    {
      struct VulkanBackend : public IService
      {
        VulkanBackend(VulkanInstance i, Devices::PhysicalDevice pd,
                      Devices::LogicalDevice ld, Memory::Allocator a)
            : instance{i}, physicalDevice(pd), logicalDevice{ld}, allocator{a} {}

        VulkanInstance instance;
        Devices::PhysicalDevice physicalDevice;
        Devices::LogicalDevice logicalDevice;
        Memory::Allocator allocator;
      };

    } // namespace Vulkan
  } // namespace Graphics
} // namespace Entropy
