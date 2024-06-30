#pragma once

#include <config.hpp>
#include <iostream>
#include <set>
#include <string>
#include <vulkan/vulkan.hpp>

#include <graphics/devices/logical_device.hpp>
#include <graphics/vulkan/queuefamilies/queuefamily.hpp>
#include <graphics/vulkan/surfaces/surface.hpp>
#include <services/service.hpp>

using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::QueueFamilies;
using namespace Entropy::Graphics::Devices;
using namespace Entropy::Services;

namespace Entropy {
namespace Graphics {
namespace DescriptorPools {
class DescriptorPool : public Service {
public:
  DescriptorPool(std::shared_ptr<LogicalDevice> logicalDevice);
  inline VkDescriptorPool Get() { return _descriptorPool; };

private:
  VkDescriptorPool _descriptorPool;
};
} // namespace DescriptorPools
} // namespace Graphics
} // namespace Entropy