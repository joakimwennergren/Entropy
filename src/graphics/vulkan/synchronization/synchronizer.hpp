#pragma once

#include <vector>

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

#include <graphics/vulkan/devices/logical_device.hpp>

using namespace Entropy::Graphics::Vulkan::Devices;

namespace Entropy
{
  namespace Graphics
  {
    namespace Vulkan
    {
      namespace Synchronization
      {
        class Synchronizer
        {
        public:
          Synchronizer(unsigned int numObjects)
          {
            ServiceLocator *sl = ServiceLocator::GetInstance();
            _logicalDevice = sl->getService<ILogicalDevice>();

            assert(logicalDevice != nullptr);
            assert(numObjects != 0);

            _imageSemaphores.resize(numObjects);
            _renderFinishedSemaphores.resize(numObjects);
            _fences.resize(numObjects);

            VkSemaphoreCreateInfo semaphoreInfo{};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            VkFenceCreateInfo fenceInfo{};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            for (size_t i = 0; i < _numObjects; i++)
            {
              if (vkCreateSemaphore(_logicalDevice->Get(), &semaphoreInfo, nullptr,
                                    &_imageSemaphores[i]) != VK_SUCCESS ||
                  vkCreateSemaphore(_logicalDevice->Get(), &semaphoreInfo, nullptr,
                                    &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
                  vkCreateFence(_logicalDevice->Get(), &fenceInfo, nullptr,
                                &_fences[i]) != VK_SUCCESS)
              {
                spdlog::error("Could not create synchronizer objects.");
              }
            }
          }

          ~Synchronizer()
          {
            for (size_t i = 0; i < _numObjects; i++)
            {
              vkDestroySemaphore(_logicalDevice->Get(), _imageSemaphores[i], nullptr);
              vkDestroySemaphore(_logicalDevice->Get(), _renderFinishedSemaphores[i],
                                 nullptr);
              vkDestroyFence(_logicalDevice->Get(), _fences[i], nullptr);
            }
          }

          inline std::vector<VkSemaphore> GetImageSemaphores()
          {
            return _imageSemaphores;
          };
          inline std::vector<VkSemaphore> GetRenderFinishedSemaphores()
          {
            return _renderFinishedSemaphores;
          };
          inline std::vector<VkFence> GetFences() { return _fences; };

        private:
          std::vector<VkSemaphore> _imageSemaphores;
          std::vector<VkSemaphore> _renderFinishedSemaphores;
          std::vector<VkFence> _fences;
          unsigned int _numObjects;
          std::shared_ptr<ILogicalDevice> _logicalDevice;
        };
      } // namespace Synchronization
    } // namespace Vulkan
  } // namespace Graphics
} // namespace Entropy
