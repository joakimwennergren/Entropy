#pragma once

#include <vulkan/vulkan.hpp>
#include <graphics/vulkan/utilities/helpers.hpp>
#include <graphics/vulkan/descriptorpools/idescriptorpool.hpp>
#include <graphics/vulkan/descriptorsetlayouts/descriptorsetlayout.hpp>
#include <graphics/vulkan/devices/ilogical_device.hpp>

using namespace Entropy::Graphics::Vulkan::DescriptorSetLayouts;

namespace Entropy::Graphics::Vulkan::DescriptorSets {
  /**
   * Manages a collection of descriptors for a particular descriptor set layout.
   *
   * Provides functionality to allocate and manage the descriptor sets used in a
   * graphics or compute pipeline. It handles binding resources like buffers,
   * images, and samplers to the appropriate binding points defined in the layout.
   */
  struct DescriptorSet {
    /**
     * Constructs a DescriptorSet object and allocates descriptor sets.
     *
     * @param layout The DescriptorSetLayout used to create the descriptor sets.
     * @return An initialized instance of DescriptorSet with allocated descriptor sets.
     */
    explicit DescriptorSet(const DescriptorSetLayout layout) {
      const ServiceLocator *sl = ServiceLocator::GetInstance();
      _logicalDevice = sl->getService<ILogicalDevice>();
      _descriptorPool = sl->getService<IDescriptorPool>();

      const std::array<VkDescriptorSetLayout, 1> layouts = {layout.Get()};

      VkDescriptorSetAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      allocInfo.descriptorPool = _descriptorPool->Get();
      allocInfo.descriptorSetCount = 1;
      allocInfo.pSetLayouts = layouts.data();

      VK_CHECK(vkAllocateDescriptorSets(_logicalDevice->Get(), &allocInfo,
        &_descriptorSet));
    }

    ~DescriptorSet()
    {
      vkFreeDescriptorSets(_logicalDevice->Get(), _descriptorPool->Get(), 1, &_descriptorSet);
    }

    VkDescriptorSet Get() { return _descriptorSet; }

  private:
    VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
    std::shared_ptr<ILogicalDevice> _logicalDevice;
    std::shared_ptr<IDescriptorPool> _descriptorPool;
  };
} // namespace Entropy::Graphics::Vulkan::DescriptorSets
