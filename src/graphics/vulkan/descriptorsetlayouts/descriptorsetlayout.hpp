#pragma once

#include <vulkan/vulkan.hpp>

namespace Entropy::Graphics::Vulkan::DescriptorSetLayouts {
  struct DescriptorSetLayout {
    /**
     * Constructs a DescriptorSetLayout object by creating a Vulkan descriptor set layout
     * with the specified layout bindings and binding flags.
     *
     * @param layoutBindings A vector containing VkDescriptorSetLayoutBinding structures that describe the bindings for the descriptor set.
     * @param bindingFlags A vector containing VkDescriptorBindingFlagsEXT flags for each binding in the layoutBindings vector.
     * @return A DescriptorSetLayout object representing the created Vulkan descriptor set layout.
     */
    DescriptorSetLayout(
      const std::vector<VkDescriptorSetLayoutBinding> &layoutBindings,
      const std::vector<VkDescriptorBindingFlagsEXT> &bindingFlags) {
      const ServiceLocator *sl = ServiceLocator::GetInstance();
      const auto logicalDevice = sl->getService<ILogicalDevice>();

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

      VK_CHECK(vkCreateDescriptorSetLayout(logicalDevice->Get(), &layoutInfo,
        nullptr,
        &_descriptorSetLayout));
    }

    [[nodiscard]] VkDescriptorSetLayout Get() const { return _descriptorSetLayout; }

  private:
    VkDescriptorSetLayout _descriptorSetLayout = VK_NULL_HANDLE;
  };
} // namespace Entropy::Graphics::Vulkan::DescriptorSetLayouts
