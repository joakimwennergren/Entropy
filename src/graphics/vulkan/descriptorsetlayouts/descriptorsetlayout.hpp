#pragma once

#include <config.hpp>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

#include <graphics/vulkan/devices/logical_device.hpp>

using namespace Entropy::Graphics::Vulkan::QueueFamilies;
using namespace Entropy::Graphics::Vulkan::Devices;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace DescriptorsetLayouts {
class DescriptorsetLayout  {
public:
  DescriptorsetLayout(std::shared_ptr<LogicalDevice> logicalDevice, std::vector<VkDescriptorSetLayoutBinding> layoutBindings)
  {
      VkDescriptorSetLayoutCreateInfo layoutInfo{};
      layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
      layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
      layoutInfo.pBindings = layoutBindings.data();

      if (vkCreateDescriptorSetLayout(logicalDevice->Get(), &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS)
      {
          exit(EXIT_FAILURE);
      }
  }
  inline VkDescriptorSetLayout Get() { return _descriptorSetLayout; };

private:
  VkDescriptorSetLayout _descriptorSetLayout;
};
} // namespace DescriptorsetLayouts
} // namespace Graphics
} // namespace Entropy
}