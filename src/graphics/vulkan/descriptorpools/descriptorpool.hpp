#pragma once

#include <config.hpp>
#include <iostream>
#include <set>
#include <string>
#include <vulkan/vulkan.hpp>

#include <graphics/vulkan/devices/logical_device.hpp>
#include <graphics/vulkan/queuefamilies/queuefamily.hpp>
#include <graphics/vulkan/surfaces/surface.hpp>
#include <services/service.hpp>

using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::QueueFamilies;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Services;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace DescriptorPools {
class DescriptorPool : public Service {
public:
  DescriptorPool(std::shared_ptr<LogicalDevice> logicalDevice)
  {
      std::array<VkDescriptorPoolSize, 6> poolSizes{};

      poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      poolSizes[0].descriptorCount = 100000;

      poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
      poolSizes[1].descriptorCount = 100000;

      poolSizes[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      poolSizes[2].descriptorCount = 100000;

      poolSizes[3].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      poolSizes[3].descriptorCount = 100000;

      poolSizes[4].type = VK_DESCRIPTOR_TYPE_SAMPLER;
      poolSizes[4].descriptorCount = 100000;

      poolSizes[5].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
      poolSizes[5].descriptorCount = 100000;

      VkDescriptorPoolCreateInfo poolInfo{};
      poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
      poolInfo.pPoolSizes = poolSizes.data();
      poolInfo.maxSets = 100000;
      poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

      if (vkCreateDescriptorPool(logicalDevice->Get(), &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS)
      {
          exit(EXIT_FAILURE);
      }
  }
  inline VkDescriptorPool Get() { return _descriptorPool; };

private:
  VkDescriptorPool _descriptorPool;
};
} // namespace DescriptorPools
} // namespace Graphics
} // namespace Entropy
}