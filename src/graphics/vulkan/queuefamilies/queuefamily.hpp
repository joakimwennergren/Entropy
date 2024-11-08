#pragma once

#include <optional>
#include <vulkan/vulkan.hpp>

namespace Entropy::Graphics::Vulkan::QueueFamilies {
  struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const {
      return graphicsFamily.has_value() && presentFamily.has_value();
    }
  };

  class QueueFamily {
  public:
    static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device,
                                                VkSurfaceKHR surface);

    QueueFamily();
  };
} // namespace Entropy::Graphics::Vulkan::QueueFamilies



