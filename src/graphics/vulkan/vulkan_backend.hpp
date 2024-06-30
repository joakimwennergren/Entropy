#pragma once

#include "graphics/vulkan/devices/logical_device.hpp"
#include "graphics/vulkan/devices/physical_device.hpp"
#include "graphics/vulkan/instances/vk_instance.hpp"
#include "vulkan/vulkan_core.h"
#include <cassert>

#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>
#include <vulkan/vulkan.hpp>

#include <graphics/vulkan/memory/allocator.hpp>

namespace Entropy {
namespace Graphics {
namespace Vulkan {

struct VulkanBackend {

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
