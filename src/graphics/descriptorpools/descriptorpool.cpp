#include "descriptorpool.hpp"

using namespace Entropy::Graphics::DescriptorPools;

DescriptorPool::DescriptorPool(std::shared_ptr<LogicalDevice> logicalDevice)
{
    std::array<VkDescriptorPoolSize, 3> poolSizes{};

    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_CONCURRENT_FRAMES_IN_FLIGHT);

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_SAMPLER;
    poolSizes[1].descriptorCount = 10000;

    poolSizes[2].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    poolSizes[2].descriptorCount = 10000;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 10000;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    if (vkCreateDescriptorPool(logicalDevice->Get(), &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS)
    {
        exit(EXIT_FAILURE);
    }
}