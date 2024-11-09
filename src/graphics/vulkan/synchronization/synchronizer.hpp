#pragma once

#include <vector>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>
#include <graphics/vulkan/devices/ilogical_device.hpp>

namespace Entropy::Graphics::Vulkan::Synchronization {
  class Synchronizer {
  public:
    /**
     * Constructs a Synchronizer object which handles synchronization primitives (semaphores and fences).
     *
     * @param numObjects The number of synchronization objects to create. It must be greater than 0.
     * @return A new instance of the Synchronizer class.
     */
    explicit Synchronizer(const unsigned int numObjects) {
      const ServiceLocator *sl = ServiceLocator::GetInstance();
      _logicalDevice = sl->getService<ILogicalDevice>();

      assert(numObjects != 0);

      _numObjects = numObjects;

      _imageSemaphores.resize(numObjects);
      _renderFinishedSemaphores.resize(numObjects);
      _fences.resize(numObjects);

      VkSemaphoreCreateInfo semaphoreInfo{};
      semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

      VkFenceCreateInfo fenceInfo{};
      fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
      fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

      for (size_t i = 0; i < _numObjects; i++) {
        if (vkCreateSemaphore(_logicalDevice->Get(), &semaphoreInfo, nullptr,
                              &_imageSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(_logicalDevice->Get(), &semaphoreInfo, nullptr,
                              &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(_logicalDevice->Get(), &fenceInfo, nullptr,
                          &_fences[i]) != VK_SUCCESS) {
          spdlog::error("Could not create synchronizer objects.");
        }
      }
    }

    /**
     * Destructor for the Synchronizer class. Cleans up Vulkan synchronization primitives.
     *
     * This method ensures that the Vulkan device is idle before destroying any synchronization objects
     * (semaphores and fences) created during the lifetime of the Synchronizer instance.
     */
    ~Synchronizer() {
      vkDeviceWaitIdle(_logicalDevice->Get());
      for (size_t i = 0; i < _numObjects; i++) {
        vkDestroySemaphore(_logicalDevice->Get(), _imageSemaphores[i], nullptr);
        vkDestroySemaphore(_logicalDevice->Get(), _renderFinishedSemaphores[i],
                           nullptr);
        vkDestroyFence(_logicalDevice->Get(), _fences[i], nullptr);
      }
    }

    std::vector<VkSemaphore> GetImageSemaphores() {
      return _imageSemaphores;
    };

    std::vector<VkSemaphore> GetRenderFinishedSemaphores() {
      return _renderFinishedSemaphores;
    };

    std::vector<VkFence> GetFences() { return _fences; };

  private:
    std::vector<VkSemaphore> _imageSemaphores;
    std::vector<VkSemaphore> _renderFinishedSemaphores;
    std::vector<VkFence> _fences;
    unsigned int _numObjects;
    std::shared_ptr<ILogicalDevice> _logicalDevice;
  };
} // namespace Entropy::Graphics::Vulkan::Synchronization
