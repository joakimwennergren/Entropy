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
class DescriptorsetLayout {
public:
  DescriptorsetLayout(
      VulkanBackend backend,
      std::vector<VkDescriptorSetLayoutBinding> layoutBindings) {

    VkDescriptorBindingFlags flags =
        VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT |
        VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT;

    VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlags = {};
    bindingFlags.pBindingFlags = &flags;
    bindingFlags.sType =
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
    layoutInfo.pBindings = layoutBindings.data();
    layoutInfo.flags =
        VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;
    layoutInfo.pNext = &bindingFlags;

    if (vkCreateDescriptorSetLayout(backend.logicalDevice.Get(), &layoutInfo,
                                    nullptr,
                                    &_descriptorSetLayout) != VK_SUCCESS) {
      exit(EXIT_FAILURE);
    }
  }
  inline VkDescriptorSetLayout Get() { return _descriptorSetLayout; };

private:
  VkDescriptorSetLayout _descriptorSetLayout;
};
} // namespace DescriptorsetLayouts
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy