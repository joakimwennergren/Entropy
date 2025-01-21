#ifndef ENTROPY_SYNCHRONIZER_H
#define ENTROPY_SYNCHRONIZER_H

#include <vector>
#include <vulkan/vulkan.hpp>
#include <spdlog/spdlog.h>
#include <graphics/vulkan/devices/ilogical_device.hpp>

namespace Entropy::Graphics::Vulkan::Synchronization {
  /**
   * The Synchronizer class is responsible for managing Vulkan synchronization primitives
   * such as semaphores and fences. These objects are used to coordinate execution
   * and resource usage across multiple GPU operations.
   */
  struct Synchronizer {
    /**
     * Constructs a Synchronizer object which handles synchronization primitives (semaphores and fences).
     *
     * @param numObjects The number of synchronization objects to create. It must be greater than 0.
     * @return A new instance of the Synchronizer class.
     */
    explicit Synchronizer(const unsigned int numObjects) {
      assert(numObjects != 0);

      _logicalDevice = ServiceLocator::GetInstance()->getService<ILogicalDevice>();
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
        VK_CHECK(vkCreateSemaphore(_logicalDevice->Get(), &semaphoreInfo, nullptr,
          &_imageSemaphores[i]));
        VK_CHECK(vkCreateSemaphore(_logicalDevice->Get(), &semaphoreInfo, nullptr,
          &_renderFinishedSemaphores[i]));
        VK_CHECK(vkCreateFence(_logicalDevice->Get(), &fenceInfo, nullptr,
          &_fences[i]));
      }
    }

    /**
     * Destructor for the Synchronizer class. Ensures that all Vulkan synchronization
     * objects, such as semaphores and fences, are properly destroyed and the logical
     * device has completed all pending operations before destruction.
     */
    ~Synchronizer() {
      // @todo remove this wait
      vkDeviceWaitIdle(_logicalDevice->Get());

      for (size_t i = 0; i < _numObjects; i++) {
        vkDestroySemaphore(_logicalDevice->Get(), _imageSemaphores[i], nullptr);
        vkDestroySemaphore(_logicalDevice->Get(), _renderFinishedSemaphores[i],
                           nullptr);
        vkDestroyFence(_logicalDevice->Get(), _fences[i], nullptr);
      }
    }

    /**
     * Retrieves the collection of image semaphores used for synchronization.
     * These semaphores are typically utilized to coordinate the usage of images
     * between different stages or operations in Vulkan.
     *
     * @return A vector containing Vulkan semaphore handles for image synchronization.
     */
    std::vector<VkSemaphore> GetImageSemaphores() {
      return _imageSemaphores;
    };

    /**
     * Retrieves the collection of semaphores that signal the completion of rendering operations.
     *
     * @return A vector containing Vulkan semaphores used to indicate that rendering tasks have finished.
     */
    std::vector<VkSemaphore> GetRenderFinishedSemaphores() {
      return _renderFinishedSemaphores;
    };

    /**
     * Retrieves the collection of Vulkan fences managed by the class. Vulkan fences
     * are synchronization primitives used to coordinate GPU and CPU operations.
     *
     * @return A vector containing the Vulkan fences.
     */
    std::vector<VkFence> GetFences() { return _fences; };

  private:
    std::vector<VkSemaphore> _imageSemaphores;
    std::vector<VkSemaphore> _renderFinishedSemaphores;
    std::vector<VkFence> _fences;
    std::shared_ptr<ILogicalDevice> _logicalDevice;
    unsigned int _numObjects{};
  };
} // namespace Entropy::Graphics::Vulkan::Synchronization

#endif // ENTROPY_SYNCHRONIZER_H
