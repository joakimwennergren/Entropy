#include "descriptorset.hpp"

using namespace Entropy::Graphics::Descriptorsets;

Descriptorset::Descriptorset(std::shared_ptr<LogicalDevice> logicalDevice, std::shared_ptr<DescriptorPool> pool, std::shared_ptr<DescriptorsetLayout> layout)
{
    std::vector<VkDescriptorSetLayout> layouts(3, layout->Get());

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool->Get();
    allocInfo.descriptorSetCount = 3;
    allocInfo.pSetLayouts = layouts.data();

    _descriptorSets.resize(3);

    if (vkAllocateDescriptorSets(logicalDevice->Get(), &allocInfo, _descriptorSets.data()) != VK_SUCCESS)
    {
        std::cout << "DS FAIL" << std::endl;
        exit(EXIT_FAILURE);
    }
}