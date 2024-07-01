#pragma once
#include "../../graphics/vulkan/descriptorsets/descriptorset.hpp"

using namespace Entropy::Graphics::Vulkan;
using namespace Entropy::Graphics::Vulkan::Descriptorsets;

namespace Entropy {
namespace Factories {
namespace Vulkan {

struct DescriptorSetFactory {

  DescriptorSetFactory(VulkanBackend backend, DescriptorPool dp) : _vkBackend{backend}, _descriptorPool{dp} {
  }

  Descriptorset CreateDescriptorSet(DescriptorsetLayout layout)
  {
    return Descriptorset(_vkBackend, _descriptorPool, layout);
  }

  private:
    VulkanBackend _vkBackend;
    DescriptorPool _descriptorPool;
};

} // namespace Drawables
} // namespace Vulkan
} // namespace Graphics