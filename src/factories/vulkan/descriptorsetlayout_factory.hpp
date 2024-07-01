#pragma once
#include "../../graphics/vulkan/descriptorsetlayouts/descriptorsetlayout.hpp"

using namespace Entropy::Graphics::Vulkan;
using namespace Entropy::Graphics::Vulkan::DescriptorsetLayouts;


namespace Entropy {
namespace Factories {
namespace Vulkan {

struct DescriptorSetLayoutFactory {

  DescriptorSetLayoutFactory(VulkanBackend backend) : _vkBackend{backend}  {
  }

  DescriptorsetLayout CreateLayout(std::vector<VkDescriptorSetLayoutBinding> layoutBindings)
  {
    return DescriptorsetLayout(_vkBackend, layoutBindings);
  }

  private:
    VulkanBackend _vkBackend;
};

} // namespace Drawables
} // namespace Vulkan
} // namespace Graphics