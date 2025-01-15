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
      const auto logicalDevice = sl->getService<ILogicalDevice>();
      const auto descriptorPool = sl->getService<IDescriptorPool>();

      const std::vector layouts(MAX_CONCURRENT_FRAMES_IN_FLIGHT,
                                layout.Get());

      VkDescriptorSetAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      allocInfo.descriptorPool = descriptorPool->Get();
      allocInfo.descriptorSetCount = MAX_CONCURRENT_FRAMES_IN_FLIGHT;
      allocInfo.pSetLayouts = layouts.data();

      _descriptorSets.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);

      VK_CHECK(vkAllocateDescriptorSets(logicalDevice->Get(), &allocInfo,
        _descriptorSets.data()));
    }

    std::vector<VkDescriptorSet> Get() { return _descriptorSets; }

  private:
    std::vector<VkDescriptorSet> _descriptorSets;
  };
} // namespace Entropy::Graphics::Vulkan::DescriptorSets
