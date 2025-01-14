#ifndef ENTROPY_VALIDATION_LAYERS_H
#define ENTROPY_VALIDATION_LAYERS_H

#include <vulkan/vulkan.hpp>

namespace Entropy::Graphics::Vulkan::ValidationLayers {
  class ValidationLayer {
  public:
    static bool CheckValidationLayerSupport(
      const std::vector<const char *> &validationLayers) {
      uint32_t layerCount;
      vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

      std::vector<VkLayerProperties> availableLayers(layerCount);
      vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

      for (const char *layerName: validationLayers) {
        bool layerFound = false;

        for (const auto &layerProperties: availableLayers) {
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
} // namespace Entropy::Graphics::Vulkan::ValidationLayers

#endif // ENTROPY_VALIDATION_LAYERS_H
