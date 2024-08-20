#ifndef __DESCRIPTORPOOL_HPP
#define __DESCRIPTORPOOL_HPP

#include "idescriptorpool.hpp"

#include <config.hpp>
#include <graphics/vulkan/devices/ilogical_device.hpp>
#include <graphics/vulkan/utilities/utilities.hpp>
#include <servicelocators/servicelocator.hpp>
#include <vulkan/vulkan.hpp>

using namespace Entropy::Graphics::Vulkan;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace DescriptorPools {

struct DescriptorPool : public ServiceBase<IDescriptorPool> {

  DescriptorPool() {

    ServiceLocator *sl = ServiceLocator::GetInstance();
    auto logicalDevice = sl->getService<ILogicalDevice>();

    std::array<VkDescriptorPoolSize, 3> poolSizes{};

    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = MAX_DESCRIPTOR_SETS_COUNT;

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[1].descriptorCount = MAX_DESCRIPTOR_SETS_COUNT;

    poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[2].descriptorCount = MAX_DESCRIPTOR_SETS_COUNT;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = MAX_DESCRIPTOR_SETS_COUNT;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT |
                     VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;

    VK_CHECK(vkCreateDescriptorPool(logicalDevice->Get(), &poolInfo, nullptr,
                                    &_descriptorPool));
  }

  inline VkDescriptorPool Get() { return _descriptorPool; };

private:
  VkDescriptorPool _descriptorPool = VK_NULL_HANDLE;
};
} // namespace DescriptorPools
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy

#endif /* __DESCRIPTORPOOL_HPP */
