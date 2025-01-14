#pragma once

#include "idescriptorpool.hpp"

#include <config.hpp>
#include <graphics/vulkan/devices/ilogical_device.hpp>
#include <graphics/vulkan/utilities/helpers.hpp>
#include <servicelocators/servicelocator.hpp>
#include <vulkan/vulkan.hpp>

namespace Entropy::Graphics::Vulkan::DescriptorPools {
  /**
   * @class DescriptorPool
   *
   * @brief Manages a pool of reusable descriptors for efficient allocation and deallocation.
   *
   * The DescriptorPool class provides functionality to manage and reuse descriptors
   * efficiently in order to minimize the overhead associated with descriptor allocation.
   * This can be particularly useful in scenarios where a large number of descriptors
   * are required and reallocated frequently.
   *
   * The class supports operations to allocate new descriptors, free existing ones,
   * and reset the pool to a state where all descriptors are available.
   */
  struct DescriptorPool final : ServiceBase<IDescriptorPool> {
    DescriptorPool() {
      const ServiceLocator *sl = ServiceLocator::GetInstance();
      const auto logicalDevice = sl->getService<ILogicalDevice>();

      std::array<VkDescriptorPoolSize, 3> poolSizes{};

      poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      poolSizes[0].descriptorCount = MAX_DESCRIPTOR_SETS_COUNT;

      poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
      poolSizes[1].descriptorCount = MAX_DESCRIPTOR_SETS_COUNT;

      poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      poolSizes[2].descriptorCount = MAX_DESCRIPTOR_SETS_COUNT;

      VkDescriptorPoolCreateInfo poolInfo{};
      poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
      poolInfo.pPoolSizes = poolSizes.data();
      poolInfo.maxSets = MAX_DESCRIPTOR_SETS_COUNT;
      poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT |
                       VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;

      VK_CHECK(vkCreateDescriptorPool(logicalDevice->Get(), &poolInfo, nullptr,
        &_descriptorPool));
    }

    VkDescriptorPool Get() override { return _descriptorPool; }

  private:
    VkDescriptorPool _descriptorPool = VK_NULL_HANDLE;
  };
} // namespace Entropy::Graphics::Vulkan::DescriptorPools
