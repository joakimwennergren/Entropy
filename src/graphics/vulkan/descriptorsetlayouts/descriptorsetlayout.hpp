#pragma once

#include <config.hpp>
#include <graphics/vulkan/devices/logical_device.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>
#include <vulkan/vulkan.hpp>

using namespace Entropy::Graphics::Vulkan::Devices;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace DescriptorsetLayouts {
struct DescriptorsetLayout {

  DescriptorsetLayout(VulkanBackend backend,
                      std::vector<VkDescriptorSetLayoutBinding> layoutBindings,
                      std::vector<VkDescriptorBindingFlagsEXT> bindingFlags) {

    VkDescriptorSetLayoutBindingFlagsCreateInfoEXT bindingFlagsInfo = {};
    bindingFlagsInfo.sType =
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO_EXT;
    bindingFlagsInfo.bindingCount = bindingFlags.size();
    bindingFlagsInfo.pBindingFlags = bindingFlags.data();

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
    layoutInfo.pBindings = layoutBindings.data();
    layoutInfo.flags =
        VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;
    layoutInfo.pNext = &bindingFlagsInfo;

    if (vkCreateDescriptorSetLayout(backend.logicalDevice.Get(), &layoutInfo,
                                    nullptr,
                                    &_descriptorSetLayout) != VK_SUCCESS) {
      exit(EXIT_FAILURE);
    }
  }
  inline VkDescriptorSetLayout Get() { return _descriptorSetLayout; };

private:
  VkDescriptorSetLayout _descriptorSetLayout = VK_NULL_HANDLE;
};
} // namespace DescriptorsetLayouts
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy