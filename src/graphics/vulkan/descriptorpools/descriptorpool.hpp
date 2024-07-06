#pragma once

#include "vulkan/vulkan_core.h"
#include <config.hpp>
#include <iostream>
#include <set>
#include <string>
#include <vulkan/vulkan.hpp>

#include <graphics/vulkan/devices/logical_device.hpp>
#include <graphics/vulkan/queuefamilies/queuefamily.hpp>
#include <graphics/vulkan/surfaces/surface.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>

using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::QueueFamilies;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Graphics::Vulkan;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace DescriptorPools {

// Select a binding for each descriptor type
constexpr int STORAGE_BINDING = 0;
constexpr int SAMPLER_BINDING = 1;
constexpr int IMAGE_BINDING = 2;
// Max count of each descriptor type
// You can query the max values for these with
// physicalDevice.getProperties().limits.maxDescriptrorSet*******
constexpr int STORAGE_COUNT = 65536;
constexpr int SAMPLER_COUNT = 65536;
constexpr int IMAGE_COUNT = 65536;

struct DescriptorPool {

  DescriptorPool(VulkanBackend vbe) : _backend{vbe} {
    std::array<VkDescriptorPoolSize, 3> poolSizes{};

    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = STORAGE_COUNT;

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[1].descriptorCount = STORAGE_COUNT;

    poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[2].descriptorCount = SAMPLER_COUNT;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 100;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT |
                     VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;

    if (vkCreateDescriptorPool(_backend.logicalDevice.Get(), &poolInfo, nullptr,
                               &_descriptorPool) != VK_SUCCESS) {
      exit(EXIT_FAILURE);
    }
  }

  inline VkDescriptorPool Get() { return _descriptorPool; };

private:
  VulkanBackend _backend;
  VkDescriptorPool _descriptorPool;
};
} // namespace DescriptorPools
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy