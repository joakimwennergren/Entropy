#pragma once

#include <config.hpp>
#include <iostream>
#include <set>
#include <string>
#include <vulkan/vulkan.hpp>

#include <spdlog/spdlog.h>

#include <graphics/descriptorpools/descriptorpool.hpp>
#include <graphics/descriptorsetlayouts/descriptorsetlayout.hpp>
#include <graphics/devices/logical_device.hpp>
#include <services/service.hpp>

using namespace Entropy::Graphics::Vulkan::QueueFamilies;
using namespace Entropy::Graphics::Devices;
using namespace Entropy::Graphics::DescriptorsetLayouts;
using namespace Entropy::Graphics::DescriptorPools;

namespace Entropy {
namespace Graphics {
namespace Descriptorsets {
class Descriptorset : public Service {
public:
  Descriptorset(std::shared_ptr<LogicalDevice> logicalDevice,
                std::shared_ptr<DescriptorPool> pool,
                std::shared_ptr<DescriptorsetLayout> layout);

  inline std::vector<VkDescriptorSet> Get() { return _descriptorSets; };

private:
  std::vector<VkDescriptorSet> _descriptorSets;
};
} // namespace Descriptorsets
} // namespace Graphics
} // namespace Entropy
