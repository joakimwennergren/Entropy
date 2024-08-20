#pragma once

#include "graphics/vulkan/devices/physical_device.hpp"
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

using namespace Entropy::Graphics::Vulkan::Devices;

// we want to immediately abort when there is an error. In normal engines this
// would give an error message to the user, or perform a dump of state.
#define VK_CHECK(x)                                                            \
  do {                                                                         \
    VkResult err = x;                                                          \
    if (err) {                                                                 \
      spdlog::error(err);                                                      \
      abort();                                                                 \
    }                                                                          \
  } while (0)

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Utilities {
class Utility {
public:
  /**
   * @brief
   * @param serviceLocator
   * @param typeFilter
   * @param properties
   * @return
   */
  uint32_t FindMemoryTypeIndex(PhysicalDevice physicalDevice,
                               uint32_t typeFilter,
                               VkMemoryPropertyFlags properties) {

    // Get the physical device's memory properties
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice.Get(), &memProperties);

    // Iterate over memoryproperties and return index of matched property
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
      if ((typeFilter & (1 << i)) &&
          (memProperties.memoryTypes[i].propertyFlags & properties) ==
              properties) {
        return i;
      }
    }

    return 0;
  }

  void *AlignedAlloc(size_t size, size_t alignment) {
    void *data = nullptr;
#if defined(_MSC_VER) || defined(__MINGW32__)
    data = _aligned_malloc(size, alignment);
#else
    int res = posix_memalign(&data, alignment, size);
    if (res != 0)
      data = nullptr;
#endif
    return data;
  }

  void AlignedFree(void *data) {
#if defined(_MSC_VER) || defined(__MINGW32__)
    _aligned_free(data);
#else
    free(data);
#endif
  }
};
} // namespace Utilities
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
