#include "descriptorset.hpp"

using namespace Entropy::Graphics::Descriptorsets;

Descriptorset::Descriptorset(std::shared_ptr<LogicalDevice> logicalDevice, std::shared_ptr<DescriptorPool> pool, std::shared_ptr<DescriptorsetLayout> layout)
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
        exit(EXIT_FAILURE);
    }
}