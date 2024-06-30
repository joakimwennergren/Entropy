#pragma once

#include <vector>

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

#include <graphics/vulkan/devices/logical_device.hpp>
#include <servicelocators/servicelocator.hpp>

using namespace Entropy::Graphics::Vulkan::Devices;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Synchronization {
class Synchronizer {
public:
  Synchronizer(LogicalDevice ld, unsigned int numObjects)
      : _logicalDevice{ld}, _numObjects{numObjects} {
    assert(numObjects != 0);

    _imageSemaphores.resize(numObjects);
    _renderFinishedSemaphores.resize(numObjects);
    _fences.resize(numObjects);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < _numObjects; i++) {
      if (vkCreateSemaphore(_logicalDevice.Get(), &semaphoreInfo, nullptr,
                            &_imageSemaphores[i]) != VK_SUCCESS ||
          vkCreateSemaphore(_logicalDevice.Get(), &semaphoreInfo, nullptr,
                            &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
          vkCreateFence(_logicalDevice.Get(), &fenceInfo, nullptr,
                        &_fences[i]) != VK_SUCCESS) {
        spdlog::error("Could not create synchronizer objects.");
      }
    }
  }

  ~Synchronizer() {
    vkDeviceWaitIdle(_logicalDevice.Get());

    for (size_t i = 0; i < _numObjects; i++) {
      vkDestroySemaphore(_logicalDevice.Get(), _imageSemaphores[i], nullptr);
      vkDestroySemaphore(_logicalDevice.Get(), _renderFinishedSemaphores[i],
                         nullptr);
      vkDestroyFence(_logicalDevice.Get(), _fences[i], nullptr);
    }
  }

  inline std::vector<VkSemaphore> GetImageSemaphores() {
    return _imageSemaphores;
  };
  inline std::vector<VkSemaphore> GetRenderFinishedSemaphores() {
    return _renderFinishedSemaphores;
  };
  inline std::vector<VkFence> GetFences() { return _fences; };

private:
  std::vector<VkSemaphore> _imageSemaphores;
  std::vector<VkSemaphore> _renderFinishedSemaphores;
  std::vector<VkFence> _fences;
  LogicalDevice _logicalDevice;
  unsigned int _numObjects;
};
} // namespace Synchronization
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
