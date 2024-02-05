#include "descriptorsetlayout.hpp"

using namespace Entropy::Graphics::DescriptorsetLayouts;

DescriptorsetLayout::DescriptorsetLayout(std::shared_ptr<LogicalDevice> logicalDevice, std::vector<VkDescriptorSetLayoutBinding> layoutBindings)
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