#pragma once

#include <config.hpp>
#include <iostream>
#include <vulkan/vulkan.hpp>

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace ValidationLayers {

class ValidationLayer {
public:
  /**
   * @brief Check if provided layers are supported
   * @param validationLayers std::vector of layers
   * @return bool
   */
  static inline bool CheckValidationLayerSupport(
      const std::vector<const char *> validationLayers) {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName : validationLayers) {
      bool layerFound = false;

      for (const auto &layerProperties : availableLayers) {
        if (strcmp(layerName, layerProperties.layerName) == 0) {
          layerFound = true;
          break;
        }
      }

      if (!layerFound) {
        return false;
      }
    }

    return true;
  }
};
} // namespace ValidationLayers
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
