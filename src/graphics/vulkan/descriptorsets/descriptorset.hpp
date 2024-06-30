#pragma once

#include <config.hpp>
#include <iostream>
#include <set>
#include <string>
#include <vulkan/vulkan.hpp>

#include <spdlog/spdlog.h>

#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>
#include <graphics/vulkan/descriptorsetlayouts/descriptorsetlayout.hpp>
#include <graphics/vulkan/devices/logical_device.hpp>
#include <services/service.hpp>

using namespace Entropy::Graphics::Vulkan::QueueFamilies;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Graphics::Vulkan::DescriptorsetLayouts;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Descriptorsets {
class Descriptorset : public Service {
public:
  Descriptorset(std::shared_ptr<LogicalDevice> logicalDevice, std::shared_ptr<DescriptorPool> pool, std::shared_ptr<DescriptorsetLayout> layout)
  {
      std::vector<VkDescriptorSetLayout> layouts(MAX_CONCURRENT_FRAMES_IN_FLIGHT, layout->Get());

      VkDescriptorSetAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      allocInfo.descriptorPool = pool->Get();
      allocInfo.descriptorSetCount = MAX_CONCURRENT_FRAMES_IN_FLIGHT;
      allocInfo.pSetLayouts = layouts.data();

      _descriptorSets.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);

      if (vkAllocateDescriptorSets(logicalDevice->Get(), &allocInfo, _descriptorSets.data()) != VK_SUCCESS)
      {
          spdlog::error("Failed to allocate descriptor sets");
      }
  }

  inline std::vector<VkDescriptorSet> Get() { return _descriptorSets; };

private:
  std::vector<VkDescriptorSet> _descriptorSets;
};
} // namespace Descriptorsets
} // namespace Graphics
} // namespace Entropy
}