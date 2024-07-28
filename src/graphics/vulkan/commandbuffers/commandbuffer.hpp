#pragma once

#include <graphics/vulkan/commandpools/commandpool.hpp>
#include <graphics/vulkan/synchronization/queuesync.hpp>
#include <spdlog/spdlog.h>

using namespace Entropy::Graphics::Vulkan::CommandPools;
using namespace Entropy::Graphics::Vulkan::Synchronization;

namespace Entropy
{
  namespace Graphics
  {
    namespace Vulkan
    {
      namespace CommandBuffers
      {

        /**
         * @brief CommandBuffer
         * @author Joakim Wennergren
         * @since Wed Jul 03 2024
         */
        struct CommandBuffer
        {
        public:
          /**
           * @brief Constructor for CommandBuffer
           * @param vbe VulkanBackend
           * @param qs QueueSync
           * @param cp CommandPool
           * @param level VkCommandBufferLevel
           */
          CommandBuffer(VkCommandBufferLevel level)
          {

            ServiceLocator *sl = ServiceLocator::GetInstance();
            auto logicalDevice = sl->getService<ILogicalDevice>();
            auto commandPool = sl->getService<ICommandPool>();

            assert(logicalDevice != nullptr);

            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = commandPool->Get();
            allocInfo.level = level;
            allocInfo.commandBufferCount = 1;
            if (vkAllocateCommandBuffers(logicalDevice->Get(), &allocInfo,
                                         &_commandBuffer) != VK_SUCCESS)
            {
              spdlog::warn("[CommandBuffer] Failed to allocate command buffer.");
            }
          }

          /**
           * @brief Start recording commandbuffer
           * @return (void)
           */
          void Record();

          /**
           * @brief End recording commandbuffer
           * @return (void)
           */
          void EndRecording();

          /**
           * @brief Record once
           * @return (void)
           */
          void RecordOnce();

          /**
           * @brief End Record once
           * @return (void)
           */
          void EndRecordingOnce();

          /**
           * @brief Get the command buffer handle
           * @return VkCommandBuffer
           */
          inline VkCommandBuffer Get() { return _commandBuffer; };

        private:
          // CommandBuffer handle
          VkCommandBuffer _commandBuffer = VK_NULL_HANDLE;
        };
      } // namespace CommandBuffers
    } // namespace Vulkan
  } // namespace Graphics
} // namespace Entropy