#pragma once

#include <config.hpp>
#include <graphics/vulkan/instances/vk_instance.hpp>
#include <iostream>
#include <optional>
#include <vulkan/vulkan.hpp>

using namespace Entropy::Graphics::Vulkan::Instances;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace QueueFamilies {
struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

class QueueFamily {
public:
  static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device,
                                              VkSurfaceKHR surface);
  QueueFamily();

private:
};
} // namespace QueueFamilies
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
