#pragma once

#include <config.hpp>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

#include <graphics/devices/logical_device.hpp>
#include <services/service.hpp>

using namespace Entropy::Graphics::Vulkan::QueueFamilies;
using namespace Entropy::Graphics::Devices;
using namespace Entropy::Services;

namespace Entropy {
namespace Graphics {
namespace DescriptorsetLayouts {
class DescriptorsetLayout : public Service {
public:
  DescriptorsetLayout(std::shared_ptr<LogicalDevice> logicalDevice,
                      std::vector<VkDescriptorSetLayoutBinding> layoutBindings);
  inline VkDescriptorSetLayout Get() { return _descriptorSetLayout; };

private:
  VkDescriptorSetLayout _descriptorSetLayout;
};
} // namespace DescriptorsetLayouts
} // namespace Graphics
} // namespace Entropy
